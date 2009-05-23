/*
 * Copyright 2008, by J. Richard Barnette
 */

#include "forth.h"

/*
 * primitives.c - Standard Forth words for arithmetic, logical,
 *   memory fetch/store, and stack manipulation operations.
 */


/* ! "store"		6.1.0010 CORE, p. 25 */
/* ( x a-addr -- ) */
static cell_ft
x_store(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    *(a_addr_ft)tos = POP(vm);
    return POP(vm);
}


/* * "star"		6.1.0090 CORE, p. 26 */
/* ( x1 x2 -- x ) */
static cell_ft
x_star(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return tos * POP(vm);
}


/* + "plus" 		6.1.0120 CORE, p. 26 */
/* ( x1 x2 -- x ) */
static cell_ft
x_plus(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return tos + POP(vm);
}


/* - "minus" 		6.1.0160 CORE, p. 27 */
/* ( x1 x2 -- x ) */
static cell_ft
x_minus(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return POP(vm) - tos;
}


/* < "less-than" 	6.1.0480 CORE, p. 30 */
/* ( n1 n2 -- flag ) */
static cell_ft
x_less_than(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return ((snumber_ft) POP(vm) < (snumber_ft) tos) ? F_TRUE : F_FALSE;
}


/* = "equals" 		6.1.0530 CORE, p. 31 */
/* ( n1 n2 -- flag ) */
static cell_ft
x_equals(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return ((snumber_ft) POP(vm) == (snumber_ft) tos) ? F_TRUE : F_FALSE;
}


/* > "greater-than" 	6.1.0540 CORE, p. 31 */
/* ( n1 n2 -- flag ) */
static cell_ft
x_greater_than(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return ((snumber_ft) POP(vm) > (snumber_ft) tos) ? F_TRUE : F_FALSE;
}


/* ?DUP "question-dup" 	6.1.0630 CORE, p. 32 */
/* ( x -- 0 | x x ) */
static cell_ft
x_question_dup(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    if (tos != 0) {
	CHECK_PUSH(vm, 1);
	PUSH(vm, tos);
    }
    return tos;
}


/* @ "fetch" 		6.1.0650 CORE, p. 32 */
/* ( a-addr -- x ) */
static cell_ft
x_fetch(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    return *(a_addr_ft)tos;
}


/* AND 			6.1.0720 CORE, p. 33 */
/* ( x1 x2 -- x ) */
static cell_ft
x_and(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return tos & POP(vm);
}


/* C! "c-store" 	6.1.0850 CORE, p. 34 */
/* ( char c-addr -- ) */
static cell_ft
x_c_store(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    *(c_addr_ft)tos = (char_ft) POP(vm);
    return POP(vm);
}


/* C@ "c-fetch" 	6.1.0870 CORE, p. 34 */
/* ( c-addr -- char ) */
static cell_ft
x_c_fetch(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    return *(c_addr_ft)tos;
}


/* DEPTH		6.1.1200 CORE, p. 36 */
/* ( -- +n ) */
static cell_ft
x_depth(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, tos);
    return DEPTH(vm) - 1;
}


/* DROP 		6.1.1260 CORE, p. 37 */
/* ( x -- ) */
static cell_ft
x_drop(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    return POP(vm);
}


/* DUP			6.1.1290 CORE, p. 38 */
/* ( x -- x x ) */
static cell_ft
x_dup(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    CHECK_PUSH(vm, 1);
    PUSH(vm, tos);
    return tos;
}


/* INVERT		6.1.1720 CORE, p. 41 */
/* ( x1 -- x2 ) */
static cell_ft
x_invert(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    return ~tos;
}


/* LSHIFT		6.1.1805 CORE, p. 42 */
/* ( x1 u -- x ) */
static cell_ft
x_lshift(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return POP(vm) << tos;
}


/* NEGATE		6.1.1910 CORE, p. 43 */
/* ( x1 -- x2 ) */
static cell_ft
x_negate(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    return -tos;
}


/* OR			6.1.1980 CORE, p. 43 */
/* ( x1 x2 -- x ) */
static cell_ft
x_or(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return tos | POP(vm);
}


/* OVER			6.1.1990 CORE, p. 43 */
/* ( x1 x2 -- x1 x2 x1 ) */
static cell_ft
x_over(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    CHECK_PUSH(vm, 1);
    PUSH(vm, tos);
    return PICK(vm, 1);
}


/* ROT			6.1.2160 CORE, p. 45 */
/* ( x1 x2 x3 -- x2 x3 x1 ) */
static cell_ft
x_rot(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    a_addr_ft sp = SP(vm);
    cell_ft t;

    CHECK_POP(vm, 3);
    t = sp[1]; sp[1] = sp[0]; sp[0] = tos;
    return t;
}


/* RSHIFT		6.1.2162 CORE, p. 45 */
/* ( x1 u -- x ) */
static cell_ft
x_rshift(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return POP(vm) >> tos;
}


/* SWAP			6.1.2260 CORE, p. 46 */
/* ( x1 x2 -- x2 x1 ) */
static cell_ft
x_swap(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    a_addr_ft sp = SP(vm);
    cell_ft t;

    CHECK_POP(vm, 2);
    t = sp[0]; sp[0] = tos;
    return t;
}


/* XOR			6.1.2490 CORE, p. 49 */
/* ( x1 x2 -- x ) */
static cell_ft
x_xor(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return tos ^ POP(vm);
}


defn_dt
primitive_defns[] = {
    { define_name, "!",		x_store },
    { define_name, "*",		x_star },
    { define_name, "+",		x_plus },
    { define_name, "-",		x_minus },
    { define_name, "<",		x_less_than },
    { define_name, "=",		x_equals },
    { define_name, ">",		x_greater_than },
    { define_name, "?DUP",	x_question_dup },
    { define_name, "@",		x_fetch },
    { define_name, "AND",	x_and },
    { define_name, "C!",	x_c_store },
    { define_name, "C@",	x_c_fetch },
    { define_name, "DEPTH",	x_depth },
    { define_name, "DROP",	x_drop },
    { define_name, "DUP",	x_dup },
    { define_name, "INVERT",	x_invert },
    { define_name, "LSHIFT",	x_lshift },
    { define_name, "NEGATE",	x_negate },
    { define_name, "OR",	x_or },
    { define_name, "OVER",	x_over },
    { define_name, "ROT",	x_rot },
    { define_name, "RSHIFT",	x_rshift },
    { define_name, "SWAP",	x_swap },
    { define_name, "XOR",	x_xor },
    { NULL }
};

#if 0
    */                    6.1.0100 CORE                   26
    */MOD                 6.1.0110 CORE                   26
    +!                    6.1.0130 CORE                   27
    /                     6.1.0230 CORE                   28
    /MOD                  6.1.0240 CORE                   28
    0<                    6.1.0250 CORE                   28
    0=                    6.1.0270 CORE                   28
    1+                    6.1.0290 CORE                   28
    1-                    6.1.0300 CORE                   29
    2!                    6.1.0310 CORE                   29
    2*                    6.1.0320 CORE                   29
    2/                    6.1.0330 CORE                   29
    2@                    6.1.0350 CORE                   29
    2DROP                 6.1.0370 CORE                   29
    2DUP                  6.1.0380 CORE                   29
    2OVER                 6.1.0400 CORE                   29
    2SWAP                 6.1.0430 CORE                   30
    >R                    6.1.0580 CORE                   32
    ABS                   6.1.0690 CORE                   32
    ALIGNED               6.1.0706 CORE                   33
    CELL+                 6.1.0880 CORE                   35
    CELLS                 6.1.0890 CORE                   35
    CHAR+                 6.1.0897 CORE                   35
    CHARS                 6.1.0898 CORE                   35
    FILL                  6.1.1540 CORE                   39
    FM/MOD                6.1.1561 CORE                   40
    M*                    6.1.1810 CORE                   42
    MAX                   6.1.1870 CORE                   42
    MIN                   6.1.1880 CORE                   42
    MOD                   6.1.1890 CORE                   43
    MOVE                  6.1.1900 CORE                   43
    R>                    6.1.2060 CORE                   44
    R@                    6.1.2070 CORE                   44
    S>D                   6.1.2170 CORE                   45
    SM/REM                6.1.2214 CORE                   46
    U<                    6.1.2340 CORE                   47
    UM*                   6.1.2360 CORE                   47
    UM/MOD                6.1.2370 CORE                   48
    0<>                   6.2.0260 CORE EXT               51
    0>                    6.2.0280 CORE EXT               51
    2>R                   6.2.0340 CORE EXT               51
    2R>                   6.2.0410 CORE EXT               52
    2R@                   6.2.0415 CORE EXT               52
    <>                    6.2.0500 CORE EXT               52
    ERASE                 6.2.1350 CORE EXT               55
    NIP                   6.2.1930 CORE EXT               56
    PICK                  6.2.2030 CORE EXT               57
    ROLL                  6.2.2150 CORE EXT               58
    TUCK                  6.2.2300 CORE EXT               59
    U>                    6.2.2350 CORE EXT               59
    WITHIN                6.2.2440 CORE EXT               60
#endif
