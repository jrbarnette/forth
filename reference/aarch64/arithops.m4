include(`prim.m4')dnl

PRIM(x_plus):
	POP(SCR0)
	add	TOS, SCR0, TOS
	NEXT

PRIM(x_minus):
	POP(SCR0)
	sub	TOS, SCR0, TOS
	NEXT

PRIM(x_two_star):
	lsl	TOS, TOS, `#'1
	NEXT

PRIM(x_two_slash):
	asr	TOS, TOS, `#'1
	NEXT

PRIM(x_less_than):
	POP(SCR0)
	FLAG(lt, SCR0)
	NEXT

PRIM(x_equals):
	POP(SCR0)
	FLAG(eq, SCR0)
	NEXT

PRIM(x_greater_than):
	POP(SCR0)
	FLAG(gt, SCR0)
	NEXT

PRIM(x_and):
	POP(SCR0)
	and	TOS, SCR0, TOS
	NEXT

PRIM(x_invert):
	mvn	TOS, TOS
	NEXT

PRIM(x_lshift):
	POP(SCR0)
	lsl	TOS, SCR0, TOS
	NEXT

PRIM(x_negate):
	neg	TOS, TOS
	NEXT

PRIM(x_or):
	POP(SCR0)
	orr	TOS, SCR0, TOS
	NEXT

PRIM(x_rshift):
	POP(SCR0)
	lsr	TOS, SCR0, TOS
	NEXT

PRIM(x_u_less):
	POP(SCR0)
	FLAG(lo, SCR0)
	NEXT

PRIM(x_xor):
	POP(SCR0)
	eor	TOS, SCR0, TOS
	NEXT
