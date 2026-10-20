/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include "forth.h"

/*
 * stackops.c - Standard Forth words for stack operations.
 */

/*------  ------  ------  ------  ------  ------  ------  ------
 * >R                    6.1.0580 CORE                   32
 * ?DUP                  6.1.0630 CORE                   32
 * DEPTH                 6.1.1200 CORE                   36
 * DROP                  6.1.1260 CORE                   37
 * DUP                   6.1.1290 CORE                   37
 * OVER                  6.1.1990 CORE                   42
 * R>                    6.1.2060 CORE                   43
 * R@                    6.1.2070 CORE                   43
 * ROT                   6.1.2160 CORE                   44
 * SWAP                  6.1.2260 CORE                   45
 *
 * 2>R                   6.2.0340 CORE EXT               50
 * 2R>                   6.2.0410 CORE EXT               50
 * 2R@                   6.2.0415 CORE EXT               50
 * PICK                  6.2.2030 CORE EXT               55
 * ROLL                  6.2.2150 CORE EXT               56
 *------  ------  ------  ------  ------  ------  ------  ------
 */


/* ( x -- ) ( R:  -- x ) execution semantics */
PRIM_HDLR(x_to_r)
{
    CHECK_POP(vm, 1);
    CHECK_RPUSH(vm, 1);
    RPUSH(vm, POP(vm));
    return ip;
}


/* ( x -- 0 | x x ) */
PRIM_HDLR(x_question_dup)
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


/* ( -- +n ) */
PRIM_HDLR(x_depth)
{
    CHECK_PUSH(vm, 1);
    // beware - PUSH() has side effects on MAXPOP()
    cell_ft depth = MAXPOP(vm);
    PUSH(vm, depth);
    return ip;
}


/* ( x -- ) */
PRIM_HDLR(x_drop)
{
    CHECK_POP(vm, 1);
    (void) POP(vm);
    return ip;
}


/* ( x -- x x ) */
PRIM_HDLR(x_dup)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    CHECK_PUSH(vm, 1);
    PUSH(vm, PICK(sp, 0));
    return ip;
}


/* ( x1 x2 -- x1 x2 x1 ) */
PRIM_HDLR(x_over)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    CHECK_PUSH(vm, 1);
    PUSH(vm, PICK(sp, 1));
    return ip;
}


/* ( -- x ) ( R: x -- ) execution semantics */
PRIM_HDLR(x_r_from)
{
    CHECK_RPOP(vm, 1);
    CHECK_PUSH(vm, 1);
    PUSH(vm, RPOP(vm));
    return ip;
}


/* ( -- x ) ( R: x -- x ) execution semantics */
PRIM_HDLR(x_r_fetch)
{
    CHECK_RPOP(vm, 1);
    CHECK_PUSH(vm, 1);
    PUSH(vm, PICK(RSP(vm), 0));
    return ip;
}


/* ( x1 x2 x3 -- x2 x3 x1 ) */
PRIM_HDLR(x_rot)
{
    a_addr_ft sp = SP(vm);
    cell_ft t;

    CHECK_POP(vm, 3);
    t = PICK(sp, 0);
    PICK(sp, 0) = PICK(sp, 2);
    PICK(sp, 2) = PICK(sp, 1);
    PICK(sp, 1) = t;
    return ip;
}


/* ( x1 x2 -- x2 x1 ) */
PRIM_HDLR(x_swap)
{
    a_addr_ft sp = SP(vm);
    cell_ft t;

    CHECK_POP(vm, 2);
    t = PICK(sp, 0);
    PICK(sp, 0) = PICK(sp, 1);
    PICK(sp, 1) = t;
    return ip;
}


/* ( x1 x2 -- ) ( R:  -- x1 x2 ) execution semantics */
PRIM_HDLR(x_two_to_r)
{
    CHECK_POP(vm, 2);
    CHECK_RPUSH(vm, 2);
    cell_ft *sp = SP(vm);
    cell_ft *rsp = RSP(vm);
    SET_RSP(vm, rsp, -2);
    PICK(rsp, -1) = PICK(sp, 1);
    PICK(rsp, -2) = PICK(sp, 0);
    SET_SP(vm, sp, 2);
    return ip;
}


/* ( -- x1 x2 ) ( R: x1 x2 -- ) execution semantics */
PRIM_HDLR(x_two_r_from)
{
    CHECK_PUSH(vm, 2);
    CHECK_RPOP(vm, 2);
    cell_ft *sp = SP(vm);
    cell_ft *rsp = RSP(vm);
    SET_SP(vm, sp, -2);
    PICK(sp, -1) = PICK(rsp, 1);
    PICK(sp, -2) = PICK(rsp, 0);
    SET_RSP(vm, rsp, 2);
    return ip;
}


/* ( -- x1 x2 ) ( R: x1 x2 -- x1 x2 ) execution semantics */
PRIM_HDLR(x_two_r_fetch)
{
    CHECK_PUSH(vm, 2);
    CHECK_RPOP(vm, 2);
    cell_ft *sp = SP(vm);
    cell_ft *rsp = RSP(vm);
    SET_SP(vm, sp, -2);
    PICK(sp, -1) = PICK(rsp, 1);
    PICK(sp, -2) = PICK(rsp, 0);
    return ip;
}


/* ( xu ... x0 u -- xu ... x0 xu ) */
PRIM_HDLR(x_pick)
{
    a_addr_ft sp = SP(vm);
    cell_ft t;

    CHECK_POP(vm, 1);
    t = PICK(sp, 0);
    CHECK_POP(vm, t+2);
    PICK(sp, 0) = PICK(sp, t+1);
    return ip;
}


/* ( xu xu-1 ... x0 u -- xu-1 ... x0 xu ) */
PRIM_HDLR(x_roll)
{
    CHECK_POP(vm, 1);
    a_addr_ft sp = SP(vm);
    cell_ft u = PICK(sp, 0);
    CHECK_POP(vm, u+2);
    cell_ft t = PICK(sp, u+1);
    while (u > 0) {
	PICK(sp, u+1) = PICK(sp, u);
	u--;
    }
    PICK(sp, 1) = t;
    SET_SP(vm, sp, 1);
    return ip;
}
