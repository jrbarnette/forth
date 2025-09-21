include(`prim.m4')dnl
include(`constants.m4')dnl

// The code in this file replicates/implements the declarations below
// from cforth.h.  Changes there must be matched here.
//
// struct fargs {
//     cell_ft depth;
//     cell_ft stack[FARGS_LEN];
// };
// extern cell_ft forth_execute(struct fstack *args, xt_ft xt);

define(`XFRAME_SIZE', `(3 CELLS)')dnl

define(`SAVE_REGS', `(10 CELLS)')dnl
define(`BUFFER_SIZE', `4')dnl
define(`SENTINEL', `0xfeedfacedeadbeef')dnl
define(`MAKE_SENTINEL',
       `mov	$1, 0xbeef
	movk	$1, 0xdead, lsl `#'16
	movk	$1, 0xface, lsl `#'32
	movk	$1, 0xfeed, lsl `#'48')dnl

	.section	__DATA,__const
        .p2align        3, 0x0
sentinel:
	.quad	SENTINEL
exception_xt:
	.quad	exception
exception_vip:
	.quad	exception_xt
successful_xt:
	.quad	successful
successful_vip:
	.quad	successful_xt

// Stack layout during interpretation (high addresses on top):
//	saved LR
//	saved FP			<----	FP
//
//					\ \
//	<callee saved regs>		| +---> SAVE_REGS
//					| /
//					+---> RSTACK_BUFFER
//	<saved args>			|
//	<return stack sentinels>	|
//					/
//
//					\
//	<base of return stack>		|
//	saved VXSP = 0			+---> RSTACK_SIZE
//	saved VSP = stack base		|
//	saved VIP = exception_vip	<----	initial VXSP = x21
//					|
//					/
//
//					\
//					|
//	<param stack sentinels>		+---> STACK_BUFFER
//					|
//					/
//	<base of param stack>		<----	initial VSP = sp

CDECL(forth_execute):
	// Save all registers designated in the ABI as callee-saved.
	// N.B. RSTACK_BUFFER is two cells for each of the 4 register
	// pairs we push here, plus the 4 pairs values we save below.
	stp	x28, x27, [sp, `#'-SAVE_REGS - (2 CELLS)]!
	stp	x26, x25, [sp, `#'(2 CELLS)]
	stp	x24, x23, [sp, `#'(4 CELLS)]
	stp	x22, x21, [sp, `#'(6 CELLS)]
	stp	x20, x19, [sp, `#'(8 CELLS)]
	stp	x29, x30, [sp, `#'SAVE_REGS]
	add	x29, sp, `#'SAVE_REGS

	// At entry:
	//   ARG0 = x0 = args
	//   ARG1 = x1 = xt
	// Scratch (unused args + return values):
	//   x2-x7

	MAKE_SENTINEL(x4)

	// N.B.  We're saving x0 (the args param) in cell 3 of the
	// buffer, not the sentinel
	stp	x4, x0, [sp, `#'-(2 CELLS)]!
	stp	x4, x4, [sp, `#'-(2 CELLS)]!
	mov	VRSP, sp

	sub	sp, sp, `#'(RSTACK_SIZE CELLS)
	stp	x4, x4, [sp, `#'-(2 CELLS)]!
	stp	x4, x4, [sp, `#'-(2 CELLS)]!
	mov	VSP, sp

	sub	sp, sp, `#'(STACK_SIZE CELLS)
	stp	x4, x4, [sp, `#'-(2 CELLS)]!
	stp	x4, x4, [sp, `#'-(2 CELLS)]!

	// Create the base exception frame to finish setting up the
	// return stack.
	mov	VXSP, 0
	adrp	VIP, exception_vip@PAGE
	add	VIP, VIP, exception_vip@PAGEOFF
	CATCH()

	mov	DP, x1
	ldr	SCR0, [x0]			// arg count
	add	SCR1, x0, SCR0, lsl `#'3	// arg pointer
	mov	TOS, x4
	negs	SCR0, SCR0
	b.eq	start_forth
copy_args_in:
	PUSH(TOS)
	adds	SCR0, SCR0, `#'1
	ldr	TOS, [SCR1, SCR0, lsl `#'3]
	b.ne	copy_args_in

start_forth:
	adrp	VIP, successful_vip@PAGE
	add	VIP, VIP, successful_vip@PAGEOFF
	EXECUTE

exception:
	mov	x0, TOS
	POP(x1)
	b	finish

successful:
	mov	x1, TOS
	mov	x0, 0

finish:
	// At this point:
	//   x0 = Exception code (or 0 for no exception)
	//       This register will become the return value
	//   x1 = Top of stack
	ldr	x2, [x29, `#'-SAVE_REGS-8]	// args ptr
	add	x3, sp, `#'((BUFFER_SIZE+STACK_SIZE) CELLS)
	subs	x3, x3, VSP			// # bytes on stack
	asr	x3, x3, `#'3			// # cells on stack
	str	x3, [x2]			// args->depth
	b.le	restore		// FIXME: ignores param stack underflow
	PUSH(x1)
	mov	x1, `#'FARGS_LEN
	cmp	x3, x1
	csel	x3, x3, x1, lt
copy_args_out:
	POP(x4)
	str	x4, [x2, x3, lsl `#'3]
	subs	x3, x3, `#'1
	b.ne	copy_args_out
restore:
	sub	sp, x29, `#'SAVE_REGS
	ldp	x29, x30, [sp, `#'SAVE_REGS]
	ldp	x20, x19, [sp, `#'(8 CELLS)]
	ldp	x22, x21, [sp, `#'(6 CELLS)]
	ldp	x24, x23, [sp, `#'(4 CELLS)]
	ldp	x26, x25, [sp, `#'(2 CELLS)]
	ldp	x28, x27, [sp], `#'SAVE_REGS + (2 CELLS)
	ret

PRIM(x_clear):
	add	VSP, sp, `#'((BUFFER_SIZE+STACK_SIZE) CELLS)
	MAKE_SENTINEL(TOS)
	NEXT

PRIM(x_rclear):
	sub	VRSP, x29, `#'SAVE_REGS+(BUFFER_SIZE CELLS)+XFRAME_SIZE
	mov	VXSP, VRSP
	NEXT

PRIM(x_depth):
	mov	SCR0, TOS
	add	TOS, sp, `#'((BUFFER_SIZE+STACK_SIZE) CELLS)
	sub	TOS, TOS, VSP			// # bytes on stack
	asr	TOS, TOS, `#'3			// # cells on stack
	PUSH(SCR0)
	NEXT

PRIM(x_execute):
	mov	DP, TOS
	POP(TOS)
	EXECUTE

PRIM(do_catch):
	CATCH()
	PUSH(TOS)
	mov	TOS, 0
	NEXT

PRIM(drop_catch):
	ldr	VXSP, [VRSP], `#'XFRAME_SIZE
	NEXT

PRIM(x_throw):
	tst	TOS, TOS
	b.ne	thrown
	POP(TOS)
	NEXT
thrown:
	THROW()
	NEXT

PRIM(do_colon):
	RPUSH(VIP)
	mov	VIP, DP
	NEXT

PRIM(do_create):
	RPUSH(VIP)
	PUSH(TOS)
	ldr	VIP, [DP]
	add	TOS, DP, `#'8
	NEXT

PRIM(x_exit):
	RPOP(VIP)
	NEXT

PRIM(do_constant):
	PUSH(TOS)
	ldr	TOS, [DP]
	NEXT

PRIM(do_variable):
	PUSH(TOS)
	mov	TOS, DP
	NEXT

PRIM(do_literal):
	PUSH(TOS)
	ldr	TOS, [VIP], `#'8
	NEXT

PRIM(do_s_quote):
	PUSH(TOS)
	ldr	TOS, [VIP], `#'8
	PUSH(VIP)
	add	VIP, VIP, TOS
	add	VIP, VIP, `#'7
	and	VIP, VIP, `#'-8
	NEXT

PRIM(do_c_quote):
	PUSH(TOS)
	mov	TOS, VIP
	ldrb	wSCR0, [VIP]
	add	VIP, SCR0, `#'8
	and	VIP, VIP, `#'-8
	NEXT
