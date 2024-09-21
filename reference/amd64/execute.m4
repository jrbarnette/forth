include(`prim.m4')dnl

define(`FARGS_LEN', `7')dnl

define(`XFRAME', `3*8')dnl

define(`SAVE_REGS', `5*8')dnl
define(`RSTACK_BUFFER', `(SAVE_REGS+3*8)')dnl
define(`STACK_BUFFER', `4*8')dnl
define(`RSTACK_BASE', `(RSTACK_SIZE+RSTACK_BUFFER)')dnl
define(`STACK_BASE', `(RSTACK_SIZE+RSTACK_BUFFER+STACK_BUFFER)')dnl
define(`SENTINEL', `0xfeedfacedeadbeef')dnl

        .section        __DATA,__data
        .p2align        4
exception_xt:
	.quad	exception
exception_vip:
	.quad	exception_xt
successful_xt:
	.quad	successful
successful_vip:
	.quad	successful_xt

# struct fargs {
#     cell_ft depth;
#     cell_ft stack[FARGS_LEN];
# };
# extern cell_ft forth_execute(struct fstack *args, xt_ft xt);

# Stack layout during interpretation:
#	ret addr
#	prev %ebp			<----	%ebp
#
#					\ \
#	<callee saved regs>		| +---> SAVE_REGS
#					| /
#					+---> RSTACK_BUFFER
#	<return stack sentinels>	|
#					/
#
#					\
#	<top of return stack>		|
#		.			|
#		.			+---> RSTACK_SIZE
#		.			|
#		.			<----	initial VRSP = %rbx
#	saved VXSP = 0			|
#	saved VSP = stack base		|
#	saved VIP = exception_vip	<----	initial VXSP = %r14
#					|
#					/
#
#					\
#					|
#	<saved `args`>			+---> STACK_BUFFER
#	<param stack sentinels>		|
#					/
#	<TOS dead cell>			<----	initial VSP = %rsp

CDECL(forth_execute):
	pushq	%rbp		# stack aligned
	movq	%rsp, %rbp

	# N.B. RSTACK_BUFFER is one cell for each `pushq` instruction
	# from here until we allocate space for the return stack.
	pushq	%rbx		# 1 - offset -8
	pushq	%r12		# 2 - offset -16
	pushq	%r13		# 3 - offset -24
	pushq	%r14		# 4 - offset -32
	pushq	%r15		# 5 - offset -40

	movabsq	$SENTINEL, %rax
	pushq	%rax		# 6 - offset -48
	pushq	%rax		# 7 - offset -56
	pushq	%rax		# 8 - offset -64
	subq	$RSTACK_SIZE, %rsp
	# ... RSTACK_BUFFER count ends here.

	# N.B. STACK_BUFFER is one cell for each `pushq` instruction
	# following...
	pushq	%rdi		# 1 - offset -RSTACK_SIZE-72
	pushq	%rax		# 2 - offset -RSTACK_SIZE-80
	pushq	%rax		# 3 - offset -RSTACK_SIZE-88
	pushq	%rax		# 4 - offset -RSTACK_SIZE-96
	# ... STACK_BUFFER count ends here.

	movq	%rax, TOS

	leaq	STACK_BUFFER`'(VSP), VRSP
	xorq	VXSP, VXSP

	leaq	exception_vip(%rip), SCR0
	movq	SCR0, 0(VRSP)
	movq	VSP, 8(VRSP)
	movq	VXSP, 16(VRSP)
	movq	VRSP, VXSP
	addq	$XFRAME, VRSP

	movq	(%rdi), SCR0
	leaq	8(%rdi,SCR0,8), SCR1
	negq	SCR0
	jz	start_forth
copy_args_in:
	pushq	TOS
	movq	(SCR1,SCR0,8), TOS
	incq	SCR0
	jnz	copy_args_in

start_forth:
	movq	%rsi, XT
	leaq	successful_vip(%rip), VIP
	jmpq	*(XT)

exception:
	movq	TOS, %rax
	popq	%rdx
	jmp	finish

successful:
	xorq	%rax, %rax
	movq	TOS, %rdx

finish:
	movq	-(RSTACK_BASE+8)(%rbp), %rdi
	leaq	-STACK_BASE`'(%rbp), %rsi
	subq	%rsp, %rsi
	movb	$3, %cl
	sarq	%cl, %rsi
	movq	%rsi, (%rdi)
	orq	%rsi, %rsi
	jle	restore		# FIXME: ignores param stack underflow
	pushq	%rdx
	movq	$FARGS_LEN, %rdx
	cmpq	%rdx, %rsi
	cmovgq	%rdx, %rsi
copy_args_out:
	popq	(%rdi,%rsi,8)
	decq	%rsi
	jnz	copy_args_out
restore:
	leaq	-SAVE_REGS`'(%rbp), %rsp
	popq	%r15
	popq	%r14
	popq	%r13
	popq	%r12
	popq	%rbx
	popq	%rbp
	retq

PRIM(x_clear):
	leaq	-STACK_BASE`'(%rbp), VSP
	movabsq	$SENTINEL, TOS
	NEXT

PRIM(x_rclear):
	leaq	-RSTACK_BASE`'(%rbp), VXSP
	leaq	-RSTACK_BASE+XFRAME`'(%rbp), VRSP
	NEXT

PRIM(x_depth):
	leaq	-STACK_BASE`'(%rbp), SCR0
	subq	VSP, SCR0
	pushq	TOS
	movb	$3, %cl
	sarq	%cl, SCR0
	movq	SCR0, TOS
	NEXT

PRIM(x_execute):
	movq	TOS, XT
	popq	TOS
	jmpq	*(XT)

PRIM(do_catch):
	movq	VIP, 0(VRSP)
	movq	VSP, 8(VRSP)
	movq	VXSP, 16(VRSP)
	movq	VRSP, VXSP
	addq	$XFRAME, VRSP
	pushq	TOS
	xorq	TOS, TOS
	NEXT

PRIM(drop_catch):
	subq	$XFRAME, VRSP
	movq	16(VRSP), VXSP
	NEXT

PRIM(x_throw):
	orq	TOS, TOS
	jnz	thrown
	popq	TOS
	NEXT
thrown:
	movq	VXSP, VRSP
	movq	0(VXSP), VIP
	movq	8(VXSP), VSP
	movq	16(VXSP), VXSP
	NEXT

PRIM(do_colon):
	RPUSH(VIP)
	leaq	8(XT), VIP
	NEXT

PRIM(do_create):
	RPUSH(VIP)
	pushq	TOS
	movq	8(XT), VIP
	leaq	16(XT), TOS
	NEXT

PRIM(x_exit):
	RPOP(VIP)
	NEXT

PRIM(do_constant):
	pushq	TOS
	movq	8(XT), TOS
	NEXT

PRIM(do_variable):
	pushq	TOS
	leaq	8(XT), TOS
	NEXT

PRIM(do_literal):
	lodsq
	pushq	TOS
	movq	%rax, TOS
	NEXT

PRIM(do_s_quote):
	lodsq
	pushq	TOS
	pushq	VIP
	movq	%rax, TOS
	addq	$7, %rax
	andq	$-8, %rax
	addq	%rax, VIP
	NEXT

PRIM(do_c_quote):
	pushq	TOS
	movq	VIP, TOS
	movzbq	(VIP), %rax
	addq	$8, %rax
	andq	$-8, %rax
	addq	%rax, VIP
	NEXT
