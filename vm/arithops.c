/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include "forth.h"

/*
 * arithops.c - Standard Forth words for single cell arithmetic and
 *   logical operations.
 */

/*------  ------  ------  ------  ------  ------  ------  ------
 * +                     6.1.0120 CORE                   26
 * -                     6.1.0160 CORE                   27
 * 2*                    6.1.0320 CORE                   29
 * 2/                    6.1.0330 CORE                   29
 * <                     6.1.0480 CORE                   30
 * =                     6.1.0530 CORE                   31
 * >                     6.1.0540 CORE                   31
 * AND                   6.1.0720 CORE                   33
 * INVERT                6.1.1720 CORE                   40
 * LSHIFT                6.1.1805 CORE                   41
 * NEGATE                6.1.1910 CORE                   42
 * OR                    6.1.1980 CORE                   42
 * RSHIFT                6.1.2162 CORE                   44
 * U<                    6.1.2340 CORE                   46
 * XOR                   6.1.2490 CORE                   48
 *------  ------  ------  ------  ------  ------  ------  ------
 */


/* ( x1 x2 -- x ) */
PRIM_HDLR(x_plus)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) + PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( x1 x2 -- x ) */
PRIM_HDLR(x_minus)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) - PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( n1 -- n2 ) */
PRIM_HDLR(x_two_star)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = (snumber_ft) PICK(sp, 0) << 1;
    return ip;
}


/* ( n1 -- n2 ) */
PRIM_HDLR(x_two_slash)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = (snumber_ft) PICK(sp, 0) >> 1;
    return ip;
}


/* ( n1 n2 -- flag ) */
PRIM_HDLR(x_less_than)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = -((snumber_ft) PICK(sp, 1) < (snumber_ft) PICK(sp, 0));
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( x1 x2 -- flag ) */
PRIM_HDLR(x_equals)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = -(PICK(sp, 1) == PICK(sp, 0));
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( n1 n2 -- flag ) */
PRIM_HDLR(x_greater_than)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = -((snumber_ft) PICK(sp, 1) > (snumber_ft) PICK(sp, 0));
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( x1 x2 -- x ) */
PRIM_HDLR(x_and)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) & PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( x1 -- x2 ) */
PRIM_HDLR(x_invert)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = ~PICK(sp, 0);
    return ip;
}


/* ( x1 u -- x ) */
PRIM_HDLR(x_lshift)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) << PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( x1 -- x2 ) */
PRIM_HDLR(x_negate)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = -PICK(sp, 0);
    return ip;
}


/* ( x1 x2 -- x ) */
PRIM_HDLR(x_or)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) | PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( x1 u -- x ) */
PRIM_HDLR(x_rshift)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) >> PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( u1 u2 -- flag ) */
PRIM_HDLR(x_u_less)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = -(PICK(sp, 1) < PICK(sp, 0));
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( x1 x2 -- x ) */
PRIM_HDLR(x_xor)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) ^ PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}
