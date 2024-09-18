include(`prim.m4')dnl

PRIM(x_store):
	popq	(TOS)
	popq	TOS
	NEXT

PRIM(x_fetch):
	movq	(TOS), TOS
	NEXT

PRIM(x_c_store):
	popq	%rax
	movb	%al, (TOS)
	popq	TOS
	NEXT

PRIM(x_c_fetch):
	movzbq	(TOS), TOS
	NEXT

PRIM(x_fill):
	# ( c-addr u char ) -> memset(addr, char, u);
	movq	%rsi, %rax
	movq	TOS, %rsi	# char -> ARG1
	popq	%rdx		# u -> ARG2
	popq	%rdi		# c-addr -> ARG0
	pushq	%rax

	CCALL(memset)

	popq	%rsi
	popq	TOS
	NEXT

PRIM(x_move):
	# ( addr-src addr-dst u ) -> memmove(dst, src, u);
	movq	%rsi, %rax
	movq	TOS, %rdx	# u -> ARG2
	popq	%rdi		# addr-dst -> ARG0
	popq	%rsi		# addr-src -> ARG1
	pushq	%rax

	CCALL(memmove)

	popq	%rsi
	popq	TOS
	NEXT
