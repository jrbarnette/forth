define(`TOS', `%rcx')dnl
define(`XT', `%rax')dnl
define(`SCR0', `%rax')dnl
define(`SCR1', `%rdx')dnl
define(`VIP', `%rsi')dnl
define(`VRSP', `%rbx')dnl
define(`VSP', `%rsp')dnl
define(`VXSP', `%r14')dnl
define(`RPUSH',
       `movq	$1, (VRSP)
	addq	`$'8, VRSP')dnl
define(`RPOP',
       `subq	`$'8, VRSP
	movq	(VRSP), $1')dnl
define(`NEXT',
       `lodsq
	jmpq	*(%rax)')dnl
define(`CDECL',
`	.section	__TEXT,__text,regular,pure_instructions
	.globl	_$1
	.p2align	4, 0x90
_$1')dnl
define(`PRIM', `CDECL(`$1')')dnl
define(`FLAG',
       `set$1	$2
	movzbq	$2, TOS
	negq	TOS')dnl
define(`CCALL',
       `leaq	-8(%rsp), %rax
	andq	`$'-16, %rax
	movq	%rsp, (%rax)
	movq	%rax, %rsp
	callq	_$1
	movq	(%rsp), %rsp')dnl
define(`RSTACK_SIZE', `128*8')dnl
