dnl  These can be clobbered if we call out to C code:
dnl
define(`TOS',   `x0')dnl
define(`SCR0',  `x1')dnl
define(`SCR1',  `x2')dnl
define(`DP',    `x3')dnl
define(`wTOS',  `w0')dnl
define(`wSCR0', `w1')dnl
define(`wSCR1', `w2')dnl
dnl
dnl  We want these preserved across calls to C code:
dnl
define(`VIP',   `x19')dnl
define(`VRSP',  `x20')dnl
define(`VSP',   `x21')dnl
define(`VXSP',  `x22')dnl
dnl
dnl Utilities for VM operations
dnl
define(`CELLS', `* 8')dnl
define(`PUSH',  `str	$1, [VSP, `#'-8]!')dnl
define(`POP',   `ldr	$1, [VSP], `#'8')dnl
define(`RPUSH', `str	$1, [VRSP, `#'-8]!')dnl
define(`RPOP',  `ldr	$1, [VRSP], `#'8')dnl
define(`EXECUTE',
       `ldr	SCR0, [DP], #8
	br	SCR0')dnl
define(`NEXT',
       `ldr	DP, [VIP], #8
	EXECUTE')dnl
define(`CATCH',
       `RPUSH(VIP)
	RPUSH(VSP)
	RPUSH(VXSP)
	mov	VXSP, VRSP')dnl
define(`DROP_CATCH',
       `RPOP(VXSP),
	add	VRSP, #(2 CELLS)')dnl
define(`THROW',
       `mov	VRSP, VXSP
	RPOP(VXSP)
	RPOP(VSP)
	RPOP(VIP)')dnl
define(`FLAG',
       `cmp	$2, TOS
	csetm	TOS, $1')dnl
define(`CDECL',
`	.section	__TEXT,__text,regular,pure_instructions
	.globl		_$1
	.p2align	2
_$1')dnl
define(`PRIM', `CDECL(`$1')')dnl
