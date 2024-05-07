	.section	__TEXT,__text,regular,pure_instructions
define(`TOS', `%rcx')dnl
define(`XT', `%rax')dnl
define(`SCR0', `%rax')dnl
define(`SCR1', `%rdx')dnl
define(`VIP', `%rsi')dnl
define(`VRSP', `%rbx')dnl
define(`VSP', `%rsp')dnl
define(`VXSP', `%r14')dnl
define(`NEXT',
       `lodsq
	jmpq	*(%rax)')dnl
define(`PRIM',
`	.globl	_$1
	.p2align	4, 0x90
_$1')dnl
define(`FLAG',
	`set$1	$2
	movzbq	$2, TOS
	negq	TOS')dnl
