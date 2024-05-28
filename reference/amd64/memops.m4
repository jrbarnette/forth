include(`prim.m4')dnl

PRIM(x_store):
	popq	SCR0
	movq	SCR0, (TOS)
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
	movq	TOS, %rsi
	popq	%rdx
	popq	%rdi
	pushq	%rax

	leaq	-8(%rsp), %rax
	andq	$-16, %rax
	movq	%rsp, (%rax)
	movq	%rax, %rsp

	callq	_memset
	movq	(%rsp), %rsp

	popq	%rsi
	NEXT

PRIM(x_move):
	# ( addr-src addr-dst u ) -> memmove(dst, src, u);
	movq	%rsi, %rax
	movq	TOS, %rdx
	popq	%rdi
	popq	%rsi
	pushq	%rax

	leaq	-8(%rsp), %rax
	andq	$-16, %rax
	movq	%rsp, (%rax)
	movq	%rax, %rsp

	callq	_memmove
	movq	(%rsp), %rsp

	popq	%rsi
	NEXT
