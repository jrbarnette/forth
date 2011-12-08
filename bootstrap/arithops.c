/*
 * Copyright 2011, by J. Richard Barnette
 */

#include "forth.h"

/*
 * arithops.c - Standard Forth words for single cell arithmetic and
 *   logical operations.
 */


/* +                     6.1.0120 CORE                   26 */
/* ( x1 x2 -- x ) */
static vminstr_p
x_plus(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) + PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* -                     6.1.0160 CORE                   27 */
/* ( x1 x2 -- x ) */
static vminstr_p
x_minus(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) - PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* 0<                    6.1.0250 CORE                   28 */
/* ( n -- flag ) */
static vminstr_p
x_zero_less(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = -((snumber_ft) PICK(sp, 0) < 0);
    return ip;
}


/* 0=                    6.1.0270 CORE                   28 */
/* ( x -- flag ) */
static vminstr_p
x_zero_equals(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = -((snumber_ft) PICK(sp, 0) == 0);
    return ip;
}


/* 1+                    6.1.0290 CORE                   28 */
/* ( n1 -- n2 ) */
static vminstr_p
x_one_plus(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = PICK(sp, 0) + 1;
    return ip;
}


/* 1-                    6.1.0300 CORE                   29 */
/* ( n1 -- n2 ) */
static vminstr_p
x_one_minus(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = PICK(sp, 0) - 1;
    return ip;
}


/* 2*                    6.1.0320 CORE                   29 */
/* ( n1 -- n2 ) */
static vminstr_p
x_two_star(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = (snumber_ft) PICK(sp, 0) << 1;
    return ip;
}


/* 2/                    6.1.0330 CORE                   29 */
/* ( n1 -- n2 ) */
static vminstr_p
x_two_slash(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = (snumber_ft) PICK(sp, 0) >> 1;
    return ip;
}


/* <                     6.1.0480 CORE                   30 */
/* ( n1 n2 -- flag ) */
static vminstr_p
x_less_than(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = -((snumber_ft) PICK(sp, 1) < (snumber_ft) PICK(sp, 0));
    SET_SP(vm, sp, 1);
    return ip;
}


/* =                     6.1.0530 CORE                   31 */
/* ( x1 x2 -- flag ) */
static vminstr_p
x_equals(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = -(PICK(sp, 1) == PICK(sp, 0));
    SET_SP(vm, sp, 1);
    return ip;
}


/* >                     6.1.0540 CORE                   31 */
/* ( n1 n2 -- flag ) */
static vminstr_p
x_greater_than(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = -((snumber_ft) PICK(sp, 1) > (snumber_ft) PICK(sp, 0));
    SET_SP(vm, sp, 1);
    return ip;
}


/* ABS                   6.1.0690 CORE                   32 */
/* ( n -- u ) */
static vminstr_p
x_abs(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    snumber_ft n;
    CHECK_POP(vm, 1);
    n = (snumber_ft) PICK(sp, 0);
    PICK(sp, 0) = (n >= 0) ? n : -n;
    return ip;
}


/* AND                   6.1.0720 CORE                   33 */
/* ( x1 x2 -- x ) */
static vminstr_p
x_and(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) & PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* INVERT                6.1.1720 CORE                   40 */
/* ( x1 -- x2 ) */
static vminstr_p
x_invert(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = ~PICK(sp, 0);
    return ip;
}


/* LSHIFT                6.1.1805 CORE                   41 */
/* ( x1 u -- x ) */
static vminstr_p
x_lshift(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) << PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* MAX                   6.1.1870 CORE                   42 */
/* ( n1 n2 -- n ) */
static vminstr_p
x_max(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    snumber_ft n1, n2;
    CHECK_POP(vm, 2);
    n1 = PICK(sp, 1);
    n2 = PICK(sp, 0);
    PICK(sp, 1) = (cell_ft) ((n1 > n2) ? n1 : n2);
    SET_SP(vm, sp, 1);
    return ip;
}


/* MIN                   6.1.1880 CORE                   42 */
/* ( n1 n2 -- n ) */
static vminstr_p
x_min(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    snumber_ft n1, n2;
    CHECK_POP(vm, 2);
    n1 = PICK(sp, 1);
    n2 = PICK(sp, 0);
    PICK(sp, 1) = (cell_ft) ((n1 < n2) ? n1 : n2);
    SET_SP(vm, sp, 1);
    return ip;
}


/* NEGATE                6.1.1910 CORE                   42 */
/* ( x1 -- x2 ) */
static vminstr_p
x_negate(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = -PICK(sp, 0);
    return ip;
}


/* OR                    6.1.1980 CORE                   42 */
/* ( x1 x2 -- x ) */
static vminstr_p
x_or(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) | PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* RSHIFT                6.1.2162 CORE                   44 */
/* ( x1 u -- x ) */
static vminstr_p
x_rshift(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) >> PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* S>D                   6.1.2170 CORE                   44 */
/* ( n -- d ) */
static vminstr_p
x_s_to_d(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    CHECK_PUSH(vm, 1);
    snumber_ft n = PICK(sp, 0);
    PUSH(vm, n >> (8 * sizeof (snumber_ft) - 1));
    return ip;
}


/* U<                    6.1.2340 CORE                   46 */
/* ( u1 u2 -- flag ) */
static vminstr_p
x_u_less(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = -(PICK(sp, 1) < PICK(sp, 0));
    SET_SP(vm, sp, 1);
    return ip;
}


/* XOR                   6.1.2490 CORE                   48 */
/* ( x1 x2 -- x ) */
static vminstr_p
x_xor(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) ^ PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* 0<>                   6.2.0260 CORE EXT               49 */
/* ( x -- flag ) */
static vminstr_p
x_zero_not_equals(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = -((snumber_ft) PICK(sp, 0) != 0);
    return ip;
}


/* 0>                    6.2.0280 CORE EXT               50 */
/* <>                    6.2.0500 CORE EXT               50 */
/* U>                    6.2.2350 CORE EXT               57 */
/* WITHIN                6.2.2440 CORE EXT               58 */


defn_dt
arithops_defns[] = {
    { define_name, "+",		x_plus },
    { define_name, "-",		x_minus },
    { define_name, "0<",	x_zero_less },
    { define_name, "0=",	x_zero_equals },
    { define_name, "1+",	x_one_plus },
    { define_name, "1-",	x_one_minus },
    { define_name, "2*",	x_two_star },
    { define_name, "2/",	x_two_slash },
    { define_name, "<",		x_less_than },
    { define_name, "=",		x_equals },
    { define_name, ">",		x_greater_than },
    { define_name, "ABS",	x_abs },
    { define_name, "AND",	x_and },
    { define_name, "INVERT",	x_invert },
    { define_name, "LSHIFT",	x_lshift },
    { define_name, "MAX",	x_max },
    { define_name, "MIN",	x_min },
    { define_name, "NEGATE",	x_negate },
    { define_name, "OR",	x_or },
    { define_name, "RSHIFT",	x_rshift },
    { define_name, "S>D",	x_s_to_d },
    { define_name, "U<",	x_u_less },
    { define_name, "XOR",	x_xor },

    { define_name, "0<>",	x_zero_not_equals },
    { NULL }
};
