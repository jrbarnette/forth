// Copyright 2025, by J. Richard Barnette. All Rights Reserved.
dnl
dnl
include(`vm.m4')dnl

PRIM(x_to_r):
	RPUSH(TOS)
	POP(TOS)
	NEXT

PRIM(x_question_dup):
	cbz	TOS, .qdup_end
	PUSH(TOS)
.qdup_end:
	NEXT

PRIM(x_drop):
	POP(TOS)
	NEXT

PRIM(x_dup):
	PUSH(TOS)
	NEXT

PRIM(x_over):
	PUSH(TOS)
	ldr	TOS, [VSP, `#'8]
	NEXT

PRIM(x_r_from):
	PUSH(TOS)
	RPOP(TOS)
	NEXT

PRIM(x_r_fetch):
	PUSH(TOS)
	ldr	TOS, [VRSP]
	NEXT

PRIM(x_rot):
	ldr	SCR0, [VSP]
	ldr	SCR1, [VSP, `#'8]
	str	SCR0, [VSP, `#'8]
	str	TOS, [VSP]
	mov	TOS, SCR1
	NEXT

PRIM(x_swap):
	ldr	SCR0, [VSP]
	str	TOS, [VSP]
	mov	TOS, SCR0
	NEXT

PRIM(x_two_to_r):
	POP(SCR0)
	RPUSH(SCR0)
	RPUSH(TOS)
	POP(TOS)
	NEXT

PRIM(x_two_r_from):
	PUSH(TOS)
	RPOP(TOS)
	RPOP(SCR0)
	PUSH(SCR0)
	NEXT

PRIM(x_two_r_fetch):
	PUSH(TOS)
	ldr	TOS, [VRSP]
	ldr	SCR0, [VRSP, `#'8]
	PUSH(SCR0)
	NEXT

PRIM(x_pick):
	ldr	TOS, [VSP, TOS, lsl `#'3]
	NEXT

PRIM(x_roll):
	cbz	TOS, .droproll
	add	SCR1, VSP, TOS, lsl `#'3
	ldr	TOS, [SCR1]
.roll:
	ldr	SCR0, [SCR1, `#'-8]!
	str	SCR0, [SCR1, `#'8]
	cmp	SCR1, VSP
	b.gt	.roll
	add	VSP, VSP, `#'8
	NEXT
.droproll:
	POP(TOS)
	NEXT
