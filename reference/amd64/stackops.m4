include(`prim.m4')dnl

PRIM(x_to_r):
	addq	$8, VRSP
	movq	TOS, (VRSP)
	popq	TOS
	NEXT

PRIM(x_question_dup):
	jrcxz	.qdup_end
	pushq	TOS
.qdup_end:
	NEXT

PRIM(x_drop):
	popq	TOS
	NEXT

PRIM(x_dup):
	pushq	TOS
	NEXT

PRIM(x_over):
	pushq	TOS
	movq	8(VSP), TOS
	NEXT

PRIM(x_r_from):
	pushq	TOS
	movq	(VRSP), TOS
	subq	$8, VRSP
	NEXT

PRIM(x_r_fetch):
	pushq	TOS
	movq	(VRSP), TOS
	NEXT

PRIM(x_rot):
	popq	SCR0
	popq	SCR1
	pushq	SCR0
	pushq	TOS
	movq	SCR1, TOS
	NEXT

PRIM(x_swap):
	popq	SCR0
	pushq	TOS
	movq	SCR0, TOS
	NEXT

PRIM(x_two_to_r):
	popq	SCR0
	movq	SCR0, 8(VRSP)
	movq	TOS, 16(VRSP)
	addq	$16, VRSP
	popq	TOS
	NEXT

PRIM(x_two_r_from):
	pushq	TOS
	subq	$16, VRSP
	movq	16(VRSP), TOS
	pushq	8(VRSP)
	NEXT

PRIM(x_two_r_fetch):
	pushq	TOS
	movq	(VRSP), TOS
	pushq	-8(VRSP)
	NEXT

PRIM(x_pick):
	movq	(VSP,TOS,8), TOS
	NEXT

PRIM(x_roll):
	jrcxz	.droproll
	movq	(VSP,TOS,8), SCR1
.roll:
	movq	-8(VSP,TOS,8), SCR0
	movq	SCR0, (VSP,TOS,8)
	loop	.roll
	addq	$8, VSP
	movq	SCR1, TOS
	NEXT
.droproll:
	popq	TOS
	NEXT
