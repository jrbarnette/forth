include(`prim.m4')dnl

# The code in this file replicates/implements the declarations below
# from cforth.h.  Changes there must be matched here.
#
# struct fargs {
#     cell_ft depth;
#     cell_ft stack[FARGS_LEN];
# };
# extern cell_ft forth_execute(struct fstack *args, xt_ft xt);

# This constant is also defined in cforth.h
define(`FARGS_LEN', `7')dnl

define(`XFRAME_VIP', `0')dnl
define(`XFRAME_VSP', `8')dnl
define(`XFRAME_VXSP', `16')dnl
define(`XFRAME_SIZE', `24')dnl

define(`SAVE_REGS', `5*8')dnl
define(`RSTACK_BUFFER', `(SAVE_REGS+3*8)')dnl
define(`STACK_BUFFER', `4*8')dnl
define(`RSTACK_BASE', `(RSTACK_SIZE+RSTACK_BUFFER)')dnl
define(`STACK_BASE', `(RSTACK_SIZE+RSTACK_BUFFER+STACK_BUFFER)')dnl
define(`SENTINEL', `0xfeedfacedeadbeef')dnl

	.section	__DATA,__const
        .p2align        4, 0x0
exception_xt:
	.quad	exception
exception_vip:
	.quad	exception_xt
successful_xt:
	.quad	successful
successful_vip:
	.quad	successful_xt

# Stack layout during interpretation (high addresses on top):
#	ret addr
#	prev %rbp			<----	%rbp
#
#					\ \
#	<callee saved regs>		| +---> SAVE_REGS
#					| /
#					+---> RSTACK_BUFFER
#	<return stack sentinels>	|
#					/
#
#					\
#	<limit of return stack>		|
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
	# At entry:
	#   ARG0 = %rdi = args
	#   ARG1 = %rsi = xt
	# Scratch (unused args + return values):
	#   %rax %rdx %rcx %r8 %r9

	# Save all registers designated in the ABI as callee-saved.
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
	# ... RSTACK_BUFFER count ends here.

	subq	$RSTACK_SIZE, %rsp	# allocate return stack
	# N.B. STACK_BUFFER is one cell for each `pushq` instruction
	# following allocation of the return stack.
	pushq	%rdi		# 1 - offset -RSTACK_SIZE-72
	pushq	%rax		# 2 - offset -RSTACK_SIZE-80
	pushq	%rax		# 3 - offset -RSTACK_SIZE-88
	pushq	%rax		# 4 - offset -RSTACK_SIZE-96
	# ... STACK_BUFFER count ends here.

	# Begin initializing VM registers here:
	#   TOS
	#   VRSP
	#   VXSP
	movq	%rax, TOS
	leaq	STACK_BUFFER`'(VSP), VRSP
	xorq	VXSP, VXSP

	leaq	exception_vip(%rip), SCR0
	movq	SCR0, XFRAME_VIP`'(VRSP)
	movq	VSP, XFRAME_VSP`'(VRSP)
	movq	VXSP, XFRAME_VXSP`'(VRSP)
	movq	VRSP, VXSP
	addq	$XFRAME_SIZE, VRSP

	movq	(%rdi), SCR0		# arg count
	leaq	8(%rdi,SCR0,8), SCR1	# arg pointer
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
	EXECUTE

exception:
	movq	TOS, %rax
	popq	%rdx
	jmp	finish

successful:
	movq	TOS, %rdx
	xorq	%rax, %rax

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
	leaq	-RSTACK_BASE+XFRAME_SIZE`'(%rbp), VRSP
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
	EXECUTE

PRIM(do_catch):
	movq	VIP, XFRAME_VIP`'(VRSP)
	movq	VSP, XFRAME_VSP`'(VRSP)
	movq	VXSP, XFRAME_VXSP`'(VRSP)
	movq	VRSP, VXSP
	addq	$XFRAME_SIZE, VRSP
	pushq	TOS
	xorq	TOS, TOS
	NEXT

PRIM(drop_catch):
	movq	XFRAME_VXSP`'(VXSP), VXSP
	leaq	XFRAME_SIZE`'(VXSP), VRSP
	NEXT

PRIM(x_throw):
	orq	TOS, TOS
	jnz	thrown
	popq	TOS
	NEXT
thrown:
	movq	VXSP, VRSP
	movq	XFRAME_VIP`'(VXSP), VIP
	movq	XFRAME_VSP`'(VXSP), VSP
	movq	XFRAME_VXSP`'(VXSP), VXSP
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
