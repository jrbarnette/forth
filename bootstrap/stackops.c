/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include "forth.h"

/*
 * stackops.c - Standard Forth words for stack operations.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// >R                    6.1.0580 CORE                   32
// ?DUP                  6.1.0630 CORE                   32
// DEPTH                 6.1.1200 CORE                   36
// DROP                  6.1.1260 CORE                   37
// DUP                   6.1.1290 CORE                   37
// OVER                  6.1.1990 CORE                   42
// R>                    6.1.2060 CORE                   43
// R@                    6.1.2070 CORE                   43
// ROT                   6.1.2160 CORE                   44
// SWAP                  6.1.2260 CORE                   45
//
// 2>R                   6.2.0340 CORE EXT               50
// 2R>                   6.2.0410 CORE EXT               50
// 2R@                   6.2.0415 CORE EXT               50
// PICK                  6.2.2030 CORE EXT               55
//------  ------  ------  ------  ------  ------  ------  ------


/* ( x -- ) ( R:  -- x ) execution semantics */
vminstr_p
x_to_r(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    CHECK_RPUSH(vm, 1);
    RPUSH(vm, POP(vm));
    return ip;
}


/* ( x -- 0 | x x ) */
vminstr_p
x_question_dup(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
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
vminstr_p
x_depth(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft depth = DEPTH(vm);
    CHECK_PUSH(vm, 1);
    PUSH(vm, depth);
    return ip;
}


/* ( x -- ) */
vminstr_p
x_drop(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    (void) POP(vm);
    return ip;
}


/* ( x -- x x ) */
vminstr_p
x_dup(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    CHECK_PUSH(vm, 1);
    PUSH(vm, PICK(sp, 0));
    return ip;
}


/* ( x1 x2 -- x1 x2 x1 ) */
vminstr_p
x_over(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    CHECK_PUSH(vm, 1);
    PUSH(vm, PICK(sp, 1));
    return ip;
}


/* ( -- x ) ( R: x -- ) execution semantics */
vminstr_p
x_r_from(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_RPOP(vm, 1);
    CHECK_PUSH(vm, 1);
    PUSH(vm, RPOP(vm));
    return ip;
}


/* ( -- x ) ( R: x -- x ) execution semantics */
vminstr_p
x_r_fetch(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_RPOP(vm, 1);
    CHECK_PUSH(vm, 1);
    PUSH(vm, PICK(RSP(vm), 0));
    return ip;
}


/* ( x1 x2 x3 -- x2 x3 x1 ) */
vminstr_p
x_rot(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
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
vminstr_p
x_swap(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
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
vminstr_p
x_two_to_r(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
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
vminstr_p
x_two_r_from(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
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
vminstr_p
x_two_r_fetch(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
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
vminstr_p
x_pick(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    a_addr_ft sp = SP(vm);
    cell_ft t;

    CHECK_POP(vm, 1);
    t = PICK(sp, 0);
    CHECK_POP(vm, t+2);
    PICK(sp, 0) = PICK(sp, t+1);
    return ip;
}
