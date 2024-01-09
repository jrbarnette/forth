include(`prim.m4')dnl

PRIM(do_colon):
	addq	$8, VRSP
	movq	VIP, (VRSP)
	leaq	8(XT), VIP
	NEXT

PRIM(do_constant):
	pushq	TOS
	movq	8(XT), TOS
	NEXT

PRIM(do_variable):
	pushq	TOS
	leaq	8(XT), TOS
	NEXT

PRIM(do_create):
	addq	$8, VRSP
	movq	VIP, (VRSP)
	pushq	TOS
	leaq	16(XT), TOS
	movq	8(XT), VIP
	NEXT
