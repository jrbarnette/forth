// Copyright 2025, by J. Richard Barnette. All Rights Reserved.
dnl
dnl
include(`vm.m4')dnl

PRIM(do_skip):
	movq	(VIP), SCR0
	leaq	(VIP,SCR0,8), VIP
	NEXT

PRIM(do_fskip):
	movq	$1, SCR0
	orq	TOS, TOS
	cmovz	(VIP), SCR0
	leaq	(VIP,SCR0,8), VIP
	popq	TOS
	NEXT

PRIM(do_plus_loop):
	addq	TOS, (VRSP)
	FLAG(o, %al)
	NEXT

PRIM(do_do):
	# ( lim idx -- ) ( R: lim-(1<<63) idx-lim+(1<<63) )
	popq	SCR0
	movq	$-1, SCR1
	shrq	$1, SCR1
	incq	SCR1
	subq	SCR1, SCR0
	subq	SCR0, TOS
	movq	SCR0, 8(VRSP)
	movq	TOS, 16(VRSP)
	addq	$16, VRSP
	popq	TOS
	NEXT

PRIM(x_i):
	pushq	TOS
	movq	(VRSP), TOS
	addq	-8(VRSP), TOS
	NEXT

PRIM(x_j):
	pushq	TOS
	movq	-16(VRSP), TOS
	addq	-24(VRSP), TOS
	NEXT

PRIM(x_unloop):
	subq	$16, VRSP
	NEXT
