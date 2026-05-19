// Copyright 2025, by J. Richard Barnette. All Rights Reserved.
dnl
dnl
include(`vm.m4')dnl

divide_by_zero:
	mov	TOS, `#'-10
	THROW

PRIM(x_star):
	POP(SCR0)
	mul	TOS, TOS, SCR0
	NEXT

PRIM(x_star_slash):
	// ( n1 n2 v -- quo )
	cbz	TOS, divide_by_zero
	NEXT

PRIM(x_star_slash_mod):
	// ( n1 n2 v -- rem quo )
	cbz	TOS, divide_by_zero
	NEXT

PRIM(x_slash):
	cbz	TOS, divide_by_zero
	POP(SCR0)
	sdiv	TOS, SCR0, TOS
	NEXT

PRIM(x_slash_mod):
	// ( n v -- rem quo )
	cbz	TOS, divide_by_zero
	POP(SCR0)
	sdiv	SCR1, SCR0, TOS
	msub	SCR0, SCR1, TOS, SCR0
	mov	TOS, SCR1
	PUSH(SCR0)
	NEXT

PRIM(x_f_m_slash_mod):
	// ( n1 n2 v -- rem quo )
	// use "floored" division
	cbz	TOS, divide_by_zero
	NEXT

PRIM(x_m_star):
	POP(SCR0)
	mul	SCR1, TOS, SCR0
	smulh	TOS, TOS, SCR0
	PUSH(SCR1)
	NEXT

PRIM(x_mod):
	cbz	TOS, divide_by_zero
	POP(SCR0)
	sdiv	SCR1, SCR0, TOS
	msub	TOS, SCR1, TOS, SCR0
	NEXT

PRIM(x_s_m_slash_rem):
	// ( n1 n2 v -- rem quo )
	// use "symmetric" division
	cbz	TOS, divide_by_zero
	NEXT

PRIM(x_u_m_star):
	POP(SCR0)
	mul	SCR1, TOS, SCR0
	umulh	TOS, TOS, SCR0
	PUSH(SCR1)
	NEXT

PRIM(x_u_m_slash_mod):
	// ( n1 n2 v -- rem quo )
	// use unsigned division
	cbz	TOS, divide_by_zero
	NEXT
