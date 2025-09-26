# Copyright 2025, by J. Richard Barnette. All Rights Reserved.
dnl
dnl
define(`TOS',     `%rcx')dnl
define(`XT',      `%rax')dnl
define(`SCR0',    `%rax')dnl
define(`SCR1',    `%rdx')dnl
dnl
define(`VIP',     `%rsi')dnl
define(`VRSP',    `%rbx')dnl
define(`VSP',     `%rsp')dnl
define(`NINSTR',  `%r12')dnl
dnl
define(`RPUSH',
       `subq	`$'8, VRSP
	movq	$1, (VRSP)')dnl
define(`RPOP',
       `movq	(VRSP), $1
	addq	`$'8, VRSP')dnl
define(`EXECUTE',
       `jmpq	*(XT)')dnl
define(`NEXT',
       `lodsq
	incq	NINSTR
	EXECUTE')dnl
define(`FLAG',
       `set$1	$2
	movzbq	$2, TOS
	negq	TOS')dnl
dnl
define(`CDECL',
`	.section	__TEXT,__text,regular,pure_instructions
	.globl		_$1
	.p2align	4, 0x90
_$1')dnl
define(`PRIM', `CDECL(`$1')')dnl


# #define STACK_SIZE	2048
# #define RSTACK_SIZE	64
# 
# typedef struct vmstate *	vmstate_p;
# 
# struct vmstate {
#     cell_ft *	ip;
#     cell_ft *	sp;
#     cell_ft *	rsp;
#     cell_ft	stack[STACK_SIZE];
#     cell_ft	rstack[RSTACK_SIZE];
# };
# extern unsigned long execute(vmstate_p vm, cell_ft *entry_xt);


	.section	__DATA,__const
        .p2align        4, 0x0
finish_xt:
	.quad	finish
finish_vip:
	.quad	finish_xt


CDECL(execute):
	pushq	%rbp		# stack is now aligned
	movq	%rsp, %rbp

	pushq	%rbx
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15

	pushq	%rdi

	#   ARG0 = %rdi = vm
	#   ARG1 = %rsi = entry_xt

	# VSP omitted
	movq	2*8(%rdi), VRSP
	movq	%rsi, XT
	xorq	NINSTR, NINSTR
	xorq	TOS, TOS
	leaq	finish_vip(%rip), VIP
	EXECUTE

finish:
	leaq	-6*8(%rbp), SCR0	# function's stack base
	movq	(SCR0), %rdi		# vm pointer
	movq	8(%rdi), SCR1		# vm->sp
	movq	TOS, -8(SCR1)		# vm->sp[-1]
	subq	%rsp, SCR0
	subq	SCR0, 8(%rdi)
	movq	VRSP, 2*8(%rdi)
	movq	NINSTR, %rax
	decq	%rax

restore:
	leaq	-5*8(%rbp), %rsp
	popq	%r15
	popq	%r14
	popq	%r13
	popq	%r12
	popq	%rbx
	popq	%rbp
	retq

PRIM(x_exit):
	RPOP(VIP)
	NEXT

PRIM(do_colon):
	RPUSH(VIP)
	leaq	8(XT), VIP
	NEXT

PRIM(do_literal):
	lodsq
	pushq	TOS
	movq	%rax, TOS
	NEXT

PRIM(x_dup):
	pushq	TOS
	NEXT

PRIM(x_swap):
	popq	SCR0
	pushq	TOS
	movq	SCR0, TOS
	NEXT

PRIM(x_drop):
	popq	TOS
	NEXT

PRIM(x_minus):
	popq	SCR0
	subq	TOS, SCR0
	movq	SCR0, TOS
	NEXT

PRIM(x_zequal):
	testq	TOS, TOS
	FLAG(e, %al)
	NEXT

PRIM(do_skip):
	movq	(VIP), SCR0
	leaq	8(VIP,SCR0,8), VIP
	NEXT

PRIM(do_zskip):
	xorq	SCR0, SCR0
	orq	TOS, TOS
	cmovz	(VIP), SCR0
	leaq	8(VIP,SCR0,8), VIP
	popq	TOS
	NEXT

PRIM(do_variable):
	pushq	TOS
	leaq	8(XT), TOS
	NEXT

PRIM(x_fetch):
	movq	(TOS), TOS
	NEXT

PRIM(x_store):
	popq	(TOS)
	popq	TOS
	NEXT
