// Copyright 2025, by J. Richard Barnette. All Rights Reserved.
dnl
dnl
include(`vm.m4')dnl

// The code in this file replicates/implements the declarations below
// from cforth.h.  Changes there must be matched here.
//
// struct fargs {
//     cell_ft depth;
//     cell_ft stack[FARGS_LEN];
// };
// extern cell_ft forth_execute(struct fstack *args, xt_ft xt);

define(`SAVE_REGS', `(10 CELLS)')dnl
define(`BUFFER_SIZE', `4')dnl
define(`MAKE_SENTINEL',
       `mov	$1, `#'(SENTINEL & 0xffff)
	movk	$1, `#'((SENTINEL >> 16) & 0xffff), lsl `#'16
	movk	$1, `#'((SENTINEL >> 32) & 0xffff), lsl `#'32
	movk	$1, `#'((SENTINEL >> 48) & 0xffff), lsl `#'48')dnl
define(`LSL_CELL', `lsl `#'CELL_SHIFT')dnl
define(`CATCH',
       `RPUSH(VIP)
	RPUSH(VSP)
	RPUSH(VXSP)
	mov	VXSP, VRSP')dnl
define(`DROP_CATCH',
       `RPOP(VXSP),
	add	VRSP, `#'(2 CELLS)')dnl
define(`THROW',
       `mov	VRSP, VXSP
	RPOP(VXSP)
	RPOP(VSP)
	RPOP(VIP)')dnl
define(`DEPTH',
       `add	$1, sp, `#'((BUFFER_SIZE+STACK_SIZE) CELLS)
	sub	$1, $1, VSP			// # bytes on stack
	asr	$1, $1, `#'CELL_SHIFT		// # cells on stack')dnl

	.section	__DATA,__const
        .p2align        3, 0x0
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
//	<callee saved regs>		+---> SAVE_REGS
//					\
//					|
//	<saved args>			+---> BUFFER_SIZE CELLS
//	<return stack sentinels>	|
//					/
//	-- base of return stack --
//					\
//	saved VXSP = 0			|
//	saved VSP = stack base		+---> XFRAME_SIZE
//	saved VIP = exception_vip	|
//					/
//
//	-- start of return stack --	<----	initial VXSP
//					<----	initial VRSP = VXSP
//	<return stack cells>		+---> (RSTACK_SIZE CELLS) - XFRAME_SIZE
//	-- limit of return stack --
//
//					\
//	<param stack sentinels>		+---> BUFFER_SIZE CELLS
//					/
//
//	-- base of param stack --	<----	initial VSP
//	<return stack cells>		+---> RSTACK_SIZE CELLS
//	-- limit of param stack --
//
//	<stack sentinels>		+---> BUFFER_SIZE CELLS
//
//	-- ABI stack extent --		<----	SP

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
	// buffer, not the sentinel.  We recover it at finish, below.
	stp	x4, x4, [sp, `#'-(4 CELLS)]!
	stp	x4, x0, [sp, `#'(2 CELLS)]
	mov	VRSP, sp

	sub	sp, sp, `#'(RSTACK_SIZE CELLS)
	stp	x4, x4, [sp, `#'-(4 CELLS)]!
	stp	x4, x4, [sp, `#'(2 CELLS)]
	mov	VSP, sp

	sub	sp, sp, `#'(STACK_SIZE CELLS)
	stp	x4, x4, [sp, `#'-(4 CELLS)]!
	stp	x4, x4, [sp, `#'(2 CELLS)]

	// At this point, all of the callee-saved VM registers are
	// initialized.  Future references to calle-saved registers
	// should be done using the VM-designated names.
	mov	VXSP, 0
	adrp	VIP, exception_vip@PAGE
	add	VIP, VIP, exception_vip@PAGEOFF
	CATCH()

	// Start initializing the caller-saved VM registers.  When this
	// is done, register references should only use the VM-designated
	// names.

	mov	DP, x1
	ldr	SCR0, [x0]			// arg count
	add	SCR1, x0, SCR0, LSL_CELL	// arg pointer
	mov	TOS, x4
	negs	SCR0, SCR0
	b.eq	start_forth

	// From this point forward, use VM-designated register names and
	// macros for the code.
copy_args_in:
	PUSH(TOS)
	adds	SCR0, SCR0, `#'1
	ldr	TOS, [SCR1, SCR0, LSL_CELL]
	b.ne	copy_args_in

start_forth:
	adrp	VIP, successful_vip@PAGE
	add	VIP, VIP, successful_vip@PAGEOFF
	EXECUTE

	// exception and successful begin the transition back from using
	// VM-designated names to using the standard register names.
exception:
	mov	x0, TOS		// Exception code (return value)
	POP(x1)			// TOS for args vector
	b	finish

successful:
	mov	x1, TOS		// TOS for args vector
	mov	x0, 0		// Exception code (return value)

	// For caller-saved registers, we now use the standard names;
	// for callee-saved, we still use the VM-designated names.
finish:
	// At this point:
	//   x0 = Exception code (or 0 for no exception)
	//       This register will become the return value
	//   x1 = Top of stack
	ldr	x2, [x29, `#'-SAVE_REGS-8]	// args ptr
	// N.B. `DEPTH' depends on `VSP' still being live.
	DEPTH(x3)
	str	x3, [x2]			// args->depth
	tst	x3, x3
	b.le	restore		// FIXME: ignores param stack underflow
	PUSH(x1)
	mov	x1, `#'FARGS_LEN
	cmp	x3, x1
	csel	x3, x3, x1, lt
copy_args_out:
	POP(x4)
	str	x4, [x2, x3, LSL_CELL]
	subs	x3, x3, `#'1
	b.ne	copy_args_out

	// From here on, we're back to using standard register names.
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
	// XXX: We assume the base exception frame doesn't need to be
	// re-initialized.  If we wanted to do that, we'd need to
	// save/restore `VIP' and `VSP' for `CATCH'.
	sub	VRSP, x29, `#'SAVE_REGS+(BUFFER_SIZE CELLS)+XFRAME_SIZE
	mov	VXSP, VRSP
	NEXT

PRIM(x_depth):
	mov	SCR0, TOS
	DEPTH(TOS)
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
	cbnz	TOS, thrown
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
	add	TOS, DP, `#'CELL_SIZE
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
	ldr	TOS, [VIP], `#'CELL_SIZE
	NEXT

PRIM(do_s_quote):
	PUSH(TOS)
	ldr	TOS, [VIP], `#'CELL_SIZE
	PUSH(VIP)
	add	VIP, VIP, TOS
	add	VIP, VIP, `#'CELL_SIZE - 1
	and	VIP, VIP, `#'-CELL_SIZE
	NEXT

PRIM(do_c_quote):
	PUSH(TOS)
	mov	TOS, VIP
	ldrb	wSCR0, [VIP]
	add	VIP, SCR0, `#'CELL_SIZE
	and	VIP, VIP, `#'-CELL_SIZE
	NEXT
