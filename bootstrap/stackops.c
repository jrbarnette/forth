/*
 * Copyright 2011, by J. Richard Barnette
 */

#include "forth.h"

/*
 * stackops.c - Standard Forth words for stack operations.
 */


/* >R "to-r"		6.1.0580 CORE, p. 32 */
/* interpretation semantics undefined */
/* ( x -- ) ( R:  -- x ) execution semantics */
static vminstr_p
x_to_r(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    CHECK_RPUSH(vm, 1);
    RPUSH(vm, POP(vm));
    return ip;
}


/* ?DUP "question-dup" 	6.1.0630 CORE, p. 32 */
/* ( x -- 0 | x x ) */
static vminstr_p
x_question_dup(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    cell_ft x;
    CHECK_POP(vm, 1);
    x = PICK(sp, 0);
    if (x != 0) {
	CHECK_PUSH(vm, 1);
	PUSH(vm, x);
    }
    return ip;
}


/* DEPTH		6.1.1200 CORE, p. 36 */
/* ( -- +n ) */
static vminstr_p
x_depth(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft depth = DEPTH(vm);
    CHECK_PUSH(vm, 1);
    PUSH(vm, depth);
    return ip;
}


/* DROP 		6.1.1260 CORE, p. 37 */
/* ( x -- ) */
static vminstr_p
x_drop(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    (void) POP(vm);
    return ip;
}


/* DUP			6.1.1290 CORE, p. 38 */
/* ( x -- x x ) */
static vminstr_p
x_dup(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    CHECK_PUSH(vm, 1);
    PUSH(vm, PICK(sp, 0));
    return ip;
}


/* OVER			6.1.1990 CORE, p. 43 */
/* ( x1 x2 -- x1 x2 x1 ) */
static vminstr_p
x_over(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    CHECK_PUSH(vm, 1);
    PUSH(vm, PICK(sp, 1));
    return ip;
}


/* R> "r-from"		6.1.2060 CORE, p. 44 */
/* interpretation semantics undefined */
/* ( -- x ) ( R: x -- ) execution semantics */
static vminstr_p
x_r_from(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    CHECK_RPOP(vm, 1);
    CHECK_PUSH(vm, 1);
    PUSH(vm, RPOP(vm));
    return ip;
}


/* R@ "r-fetch"		6.1.2070 CORE, p. 44 */
/* interpretation semantics undefined */
/* ( -- x ) ( R: x -- x ) execution semantics */
static vminstr_p
x_r_fetch(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    CHECK_RPOP(vm, 1);
    CHECK_PUSH(vm, 1);
    PUSH(vm, PICK(RSP(vm), 0));
    return ip;
}


/* ROT			6.1.2160 CORE, p. 45 */
/* ( x1 x2 x3 -- x2 x3 x1 ) */
static vminstr_p
x_rot(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    a_addr_ft sp = SP(vm);
    cell_ft t;

    CHECK_POP(vm, 3);
    t = PICK(sp, 0);
    PICK(sp, 0) = PICK(sp, 1);
    PICK(sp, 1) = PICK(sp, 2);
    PICK(sp, 2) = t;
    return ip;
}


/* SWAP			6.1.2260 CORE, p. 46 */
/* ( x1 x2 -- x2 x1 ) */
static vminstr_p
x_swap(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    a_addr_ft sp = SP(vm);
    cell_ft t;

    CHECK_POP(vm, 2);
    t = PICK(sp, 0);
    PICK(sp, 0) = PICK(sp, 1);
    PICK(sp, 1) = t;
    return ip;
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
