include(`prim.m4')dnl

PRIM(x_execute):
	movq	TOS, %rax
	popq	TOS
	jmpq	*(%rax)

PRIM(x_exit):
	movq	(VRSP), VIP
	subq	$8, VRSP
	NEXT

PRIM(do_catch):
	movq	VIP, 8(VRSP)
	movq	VSP, 16(VRSP)
	movq	VXSP, 24(VRSP)
	addq	$24, VRSP
	movq	VRSP, VXSP
	pushq	TOS
	xorq	TOS, TOS
	NEXT

PRIM(drop_catch):
	movq	(VRSP), VXSP
	subq	$24, VRSP
	NEXT

PRIM(x_throw):
PRIM(x_clear):
PRIM(x_rclear):
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
