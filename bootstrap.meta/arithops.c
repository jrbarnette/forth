/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include "forth.h"

/*
 * arithops.c - Standard Forth words for single cell arithmetic and
 *   logical operations.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// +                     6.1.0120 CORE                   26
// -                     6.1.0160 CORE                   27
// 0<                    6.1.0250 CORE                   28
// 0=                    6.1.0270 CORE                   28
// 1+                    6.1.0290 CORE                   28
// 1-                    6.1.0300 CORE                   29
// 2*                    6.1.0320 CORE                   29
// 2/                    6.1.0330 CORE                   29
// <                     6.1.0480 CORE                   30
// =                     6.1.0530 CORE                   31
// >                     6.1.0540 CORE                   31
// ABS                   6.1.0690 CORE                   32
// AND                   6.1.0720 CORE                   33
// INVERT                6.1.1720 CORE                   40
// LSHIFT                6.1.1805 CORE                   41
// MAX                   6.1.1870 CORE                   42
// MIN                   6.1.1880 CORE                   42
// NEGATE                6.1.1910 CORE                   42
// OR                    6.1.1980 CORE                   42
// RSHIFT                6.1.2162 CORE                   44
// S>D                   6.1.2170 CORE                   44
// U<                    6.1.2340 CORE                   46
// XOR                   6.1.2490 CORE                   48
//
// 0<>                   6.2.0260 CORE EXT               49
// 0>                    6.2.0280 CORE EXT               50
// <>                    6.2.0500 CORE EXT               50
// U>                    6.2.2350 CORE EXT               57
// WITHIN                6.2.2440 CORE EXT               58
//------  ------  ------  ------  ------  ------  ------  ------


/* ( x1 x2 -- x ) */
vminstr_p
x_plus(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) + PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( x1 x2 -- x ) */
vminstr_p
x_minus(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) - PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( n1 -- n2 ) */
vminstr_p
x_two_star(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = (snumber_ft) PICK(sp, 0) << 1;
    return ip;
}


/* ( n1 -- n2 ) */
vminstr_p
x_two_slash(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = (snumber_ft) PICK(sp, 0) >> 1;
    return ip;
}


/* ( n1 n2 -- flag ) */
vminstr_p
x_less_than(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = -((snumber_ft) PICK(sp, 1) < (snumber_ft) PICK(sp, 0));
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( x1 x2 -- flag ) */
vminstr_p
x_equals(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = -(PICK(sp, 1) == PICK(sp, 0));
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( n1 n2 -- flag ) */
vminstr_p
x_greater_than(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = -((snumber_ft) PICK(sp, 1) > (snumber_ft) PICK(sp, 0));
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( x1 x2 -- x ) */
vminstr_p
x_and(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) & PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( x1 -- x2 ) */
vminstr_p
x_invert(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = ~PICK(sp, 0);
    return ip;
}


/* ( x1 u -- x ) */
vminstr_p
x_lshift(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) << PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( x1 -- x2 ) */
vminstr_p
x_negate(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = -PICK(sp, 0);
    return ip;
}


/* ( x1 x2 -- x ) */
vminstr_p
x_or(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) | PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( x1 u -- x ) */
vminstr_p
x_rshift(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) >> PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( u1 u2 -- flag ) */
vminstr_p
x_u_less(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = -(PICK(sp, 1) < PICK(sp, 0));
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( x1 x2 -- x ) */
vminstr_p
x_xor(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) ^ PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


DIRECT_FORTH(init_arith_prim) // {
    PRIM("+",		x_plus)
    PRIM("-",		x_minus)
    PRIM("2*",		x_two_star)
    PRIM("2/",		x_two_slash)
    PRIM("<",		x_less_than)
    PRIM("=",		x_equals)
    PRIM(">",		x_greater_than)
    PRIM("AND",		x_and)
    PRIM("INVERT",	x_invert)
    PRIM("LSHIFT",	x_lshift)
    PRIM("NEGATE",	x_negate)
    PRIM("OR",		x_or)
    PRIM("RSHIFT",	x_rshift)
    PRIM("U<",		x_u_less)
    PRIM("XOR",		x_xor)
END_DIRECT // }


META_FORTH(init_arith_ops) // {
    XCOLON("ABS") DUP L(0) LESS IF NEGATE THEN XSEMICOLON
END_META // }

// 0<                    6.1.0250 CORE                   28
// 0=                    6.1.0270 CORE                   28
// 1+                    6.1.0290 CORE                   28
// 1-                    6.1.0300 CORE                   29
// MAX                   6.1.1870 CORE                   42
// MIN                   6.1.1880 CORE                   42
// S>D                   6.1.2170 CORE                   44
// 0<>                   6.2.0260 CORE EXT               49
// 0>                    6.2.0280 CORE EXT               50
// <>                    6.2.0500 CORE EXT               50
// U>                    6.2.2350 CORE EXT               57
// WITHIN                6.2.2440 CORE EXT               58
