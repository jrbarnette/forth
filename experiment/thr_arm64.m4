// Copyright 2025, by J. Richard Barnette. All Rights Reserved.
dnl
dnl
define(`TOS',     `x0')dnl
define(`SCR0',    `x1')dnl
define(`SCR1',    `x2')dnl
define(`DP',      `x3')dnl
define(`wTOS',    `w0')dnl
define(`wSCR0',   `w1')dnl
define(`wSCR1',   `w2')dnl
dnl
define(`VIP',     `x19')dnl
define(`VRSP',    `x20')dnl
define(`VSP',     `x21')dnl
define(`NINSTR',  `x22')dnl
dnl
define(`PUSH',  `str	$1, [VSP, `#'-8]!')dnl
define(`POP',   `ldr	$1, [VSP], `#'8')dnl
define(`RPUSH', `str	$1, [VRSP, `#'-8]!')dnl
define(`RPOP',  `ldr	$1, [VRSP], `#'8')dnl
dnl
define(`EXECUTE',
       `ldr	SCR0, [DP], `#'8
	br	SCR0')dnl
define(`NEXT',
       `ldr	DP, [VIP], `#'8
	add	NINSTR, NINSTR, 1
	EXECUTE')dnl
define(`FLAG',
       `cmp	$2, TOS
	csetm	TOS, $1')dnl
define(`CDECL',
`	.section	__TEXT,__text,regular,pure_instructions
	.globl		_$1
	.p2align	2
_$1')dnl
define(`PRIM', `CDECL(`$1')')dnl


// #define STACK_SIZE	2048
// #define RSTACK_SIZE	64
// 
// typedef struct vmstate *	vmstate_p;
// 
// struct vmstate {
//     cell_ft *	ip;
//     cell_ft *	sp;
//     cell_ft *	rsp;
//     cell_ft	stack[STACK_SIZE];
//     cell_ft	rstack[RSTACK_SIZE];
// };
// extern unsigned long execute(vmstate_p vm, cell_ft *entry_xt);


	.section	__DATA,__const
        .p2align        3, 0x0
finish_xt:
	.quad	finish
finish_vip:
	.quad	finish_xt


CDECL(execute):
	// Save all registers designated in the ABI as callee-saved.
	stp	x28, x27, [sp, `#'-6*8*2]!
	stp	x26, x25, [sp, `#'1*8*2]
	stp	x24, x23, [sp, `#'2*8*2]
	stp	x22, x21, [sp, `#'3*8*2]
	stp	x20, x19, [sp, `#'4*8*2]
	stp	x29, x30, [sp, `#'5*8*2]
	add	x29, sp, `#'5*8*2
	stp	x0, x1, [sp, `#'-8*2]!

	ldr	VSP, [x0, `#'1*8]
	ldr	VRSP, [x0, `#'2*8]
	mov	DP, x1
	mov	NINSTR, 0
	mov	TOS, 0

	adrp	VIP, finish_vip@PAGE
	add	VIP, VIP, finish_vip@PAGEOFF
	EXECUTE

finish:
	str	TOS, [VSP]
	ldr	x2, [sp]
	str	VSP, [x2, `#'1*8]
	str	VRSP, [x2, `#'2*8]
	sub	x0, NINSTR, `#'1

restore:
	sub	sp, x29, `#'5*8*2
	ldp	x29, x30, [sp, `#'5*8*2]
	ldp	x20, x19, [sp, `#'4*8*2]
	ldp	x22, x21, [sp, `#'3*8*2]
	ldp	x24, x23, [sp, `#'2*8*2]
	ldp	x26, x25, [sp, `#'1*8*2]
	ldp	x28, x27, [sp], `#'6*8*2
	ret

PRIM(x_exit):
	RPOP(VIP)
	NEXT

PRIM(do_colon):
	RPUSH(VIP)
	mov	VIP, DP
	NEXT

PRIM(do_literal):
	PUSH(TOS)
	ldr	TOS, [VIP], `#'8
	NEXT

PRIM(x_dup):
	PUSH(TOS)
	NEXT

PRIM(x_swap):
	ldr	SCR0, [VSP]
	str	TOS, [VSP]
	mov	TOS, SCR0
	NEXT

PRIM(x_drop):
	POP(TOS)
	NEXT

PRIM(x_minus):
	POP(SCR0)
	sub	TOS, SCR0, TOS
	NEXT

PRIM(x_zequal):
	FLAG(eq, xzr)
	NEXT

PRIM(do_skip):
	ldr	SCR0, [VIP], `#'8
	add	VIP, VIP, SCR0, lsl #3
	NEXT

PRIM(do_zskip):
	ldr	SCR0, [VIP], `#'8
	cbnz	TOS, .nozskip
	add	VIP, VIP, SCR0, lsl #3
.nozskip:
	POP(TOS)
	NEXT

PRIM(do_variable):
	PUSH(TOS)
	mov	TOS, DP
	NEXT

PRIM(x_fetch):
	ldr	TOS, [TOS]
	NEXT

PRIM(x_store):
	POP(SCR0)
	str	SCR0, [TOS]
	POP(TOS)
	NEXT
