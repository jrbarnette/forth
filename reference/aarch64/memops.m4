// Copyright 2025, by J. Richard Barnette. All Rights Reserved.
dnl
dnl
include(`vm.m4')dnl

PRIM(x_store):
	POP(SCR0)
	str	SCR0, [TOS]
	POP(TOS)
	NEXT

PRIM(x_fetch):
	ldr	TOS, [TOS]
	NEXT

PRIM(x_c_store):
	POP(SCR0)
	strb	wSCR0, [TOS]
	POP(TOS)
	NEXT

PRIM(x_c_fetch):
	ldrb	wTOS, [TOS]
	NEXT

PRIM(x_fill):
	# ( c-addr u char ) -> memset(addr, char, u);
	mov	x1, TOS
	POP(x2)
	POP(x0)
	CCALL(memset)
	POP(TOS)
	NEXT

PRIM(x_move):
	# ( addr-src addr-dst u ) -> memmove(dst, src, u);
	mov	x2, TOS
	POP(x0)
	POP(x1)
	CCALL(memmove)
	POP(TOS)
	NEXT
