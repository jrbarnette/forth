/*
 * Copyright 2009, by J. Richard Barnette
 */

#include "forth.h"

/*
 * stackops.c - Standard Forth words for stack operations.
 */


/* >R "to-r"		6.1.0580 CORE, p. 32 */
/* interpretation semantics undefined */
/* ( x -- ) ( R:  -- x ) execution semantics */
static cell_ft
x_to_r(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    CHECK_RPUSH(vm, 1);
    RPUSH(vm, tos);
    return POP(vm);
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


/* R> "r-from"		6.1.2060 CORE, p. 44 */
/* interpretation semantics undefined */
/* ( -- x ) ( R: x -- ) execution semantics */
static cell_ft
x_r_from(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    CHECK_RPOP(vm, 1);
    PUSH(vm, tos);
    return RPOP(vm);
}


/* R@ "r-fetch"		6.1.2070 CORE, p. 44 */
/* interpretation semantics undefined */
/* ( -- x ) ( R: x -- x ) execution semantics */
static cell_ft
x_r_fetch(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    CHECK_RPOP(vm, 1);
    PUSH(vm, tos);
    return RSP(vm)[0];
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


defn_dt
stackops_defns[] = {
    { define_name, ">R",	x_to_r, NAME_TYPE_NO_INTERPRET },
    { define_name, "?DUP",	x_question_dup },
    { define_name, "DEPTH",	x_depth },
    { define_name, "DROP",	x_drop },
    { define_name, "DUP",	x_dup },
    { define_name, "OVER",	x_over },
    { define_name, "R>",	x_r_from, NAME_TYPE_NO_INTERPRET },
    { define_name, "R@",	x_r_fetch, NAME_TYPE_NO_INTERPRET },
    { define_name, "ROT",	x_rot },
    { define_name, "SWAP",	x_swap },
    { NULL }
};

#if 0
    2DROP                 6.1.0370 CORE                   29
    2DUP                  6.1.0380 CORE                   29
    2OVER                 6.1.0400 CORE                   29
    2SWAP                 6.1.0430 CORE                   30
    2>R                   6.2.0340 CORE EXT               51
    2R>                   6.2.0410 CORE EXT               52
    2R@                   6.2.0415 CORE EXT               52
    NIP                   6.2.1930 CORE EXT               56
    PICK                  6.2.2030 CORE EXT               57
    ROLL                  6.2.2150 CORE EXT               58
    TUCK                  6.2.2300 CORE EXT               59
#endif
