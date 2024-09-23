include(`prim.m4')dnl

PRIM(x_plus):
	popq	SCR0
	addq	SCR0, TOS
	NEXT

PRIM(x_minus):
	popq	SCR0
	subq	TOS, SCR0
	movq	SCR0, TOS
	NEXT

PRIM(x_two_star):
	shlq	$1, TOS
	NEXT

PRIM(x_two_slash):
	sarq	$1, TOS
	NEXT

PRIM(x_less_than):
	popq	SCR0
	cmpq	TOS, SCR0
	FLAG(l, %al)
	NEXT

PRIM(x_equals):
	popq	SCR0
	cmpq	TOS, SCR0
	FLAG(e, %al)
	NEXT

PRIM(x_greater_than):
	popq	SCR0
	cmpq	TOS, SCR0
	FLAG(g, %al)
	NEXT

PRIM(x_and):
	popq	SCR0
	andq	SCR0, TOS
	NEXT

PRIM(x_invert):
	notq	TOS
	NEXT

PRIM(x_lshift):
	popq	SCR0
	shlq	%cl, SCR0
	movq	SCR0, TOS
	NEXT

PRIM(x_negate):
	negq	TOS
	NEXT

PRIM(x_or):
	popq	SCR0
	orq	SCR0, TOS
	NEXT

PRIM(x_rshift):
	popq	SCR0
	shrq	%cl, SCR0
	movq	SCR0, TOS
	NEXT

PRIM(x_u_less):
	popq	SCR0
	cmpq	TOS, SCR0
	FLAG(b, %al)
	NEXT

PRIM(x_xor):
	popq	SCR0
	xorq	SCR0, TOS
	NEXT
