/*
 * Copyright 2011, by J. Richard Barnette
 */

#include "forth.h"

/*
 * arithops.c - Standard Forth words for single cell arithmetic and
 *   logical operations.
 */


/* + "plus" 		6.1.0120 CORE, p. 26 */
/* ( x1 x2 -- x ) */
static vminstr_p
x_plus(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) + PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* - "minus" 		6.1.0160 CORE, p. 27 */
/* ( x1 x2 -- x ) */
static vminstr_p
x_minus(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) - PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* 0< "zero-less"	6.1.0250 CORE, p. 28 */
/* ( n -- flag ) */
static vminstr_p
x_zero_less(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = -((snumber_ft) PICK(sp, 0) < 0);
    return ip;
}


/* 0= "zero-equals"	6.1.0270 CORE, p. 28 */
/* ( x -- flag ) */
static vminstr_p
x_zero_equals(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = -((snumber_ft) PICK(sp, 0) == 0);
    return ip;
}


/* 1+ "one-plus"	6.1.0290 CORE, p. 28 */
/* ( n1 -- n2 ) */
static vminstr_p
x_one_plus(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = PICK(sp, 0) + 1;
    return ip;
}


/* 1- "one-minus"	6.1.0300 CORE, p. 29 */
/* ( n1 -- n2 ) */
static vminstr_p
x_one_minus(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = PICK(sp, 0) - 1;
    return ip;
}


/* 2* "two-star"	6.1.0320 CORE, p. 29 */
/* ( n1 -- n2 ) */
static vminstr_p
x_two_star(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = (snumber_ft) PICK(sp, 0) << 1;
    return ip;
}


/* 2/ "two-slash"	6.1.0330 CORE, p. 29 */
/* ( n1 -- n2 ) */
static vminstr_p
x_two_slash(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = (snumber_ft) PICK(sp, 0) >> 1;
    return ip;
}


/* < "less-than" 	6.1.0480 CORE, p. 30 */
/* ( n1 n2 -- flag ) */
static vminstr_p
x_less_than(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = -((snumber_ft) PICK(sp, 1) < (snumber_ft) PICK(sp, 0));
    SET_SP(vm, sp, 1);
    return ip;
}


/* = "equals" 		6.1.0530 CORE, p. 31 */
/* ( x1 x2 -- flag ) */
static vminstr_p
x_equals(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = -(PICK(sp, 1) == PICK(sp, 0));
    SET_SP(vm, sp, 1);
    return ip;
}


/* > "greater-than" 	6.1.0540 CORE, p. 31 */
/* ( n1 n2 -- flag ) */
static vminstr_p
x_greater_than(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = -((snumber_ft) PICK(sp, 1) > (snumber_ft) PICK(sp, 0));
    SET_SP(vm, sp, 1);
    return ip;
}


/* ABS			6.1.0690 CORE, p. 32 */
/* ( n -- u ) */
static vminstr_p
x_abs(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    snumber_ft n;
    CHECK_POP(vm, 1);
    n = (snumber_ft) PICK(sp, 0);
    PICK(sp, 0) = (n >= 0) ? n : -n;
    return ip;
}


/* AND 			6.1.0720 CORE, p. 33 */
/* ( x1 x2 -- x ) */
static vminstr_p
x_and(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) & PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* INVERT		6.1.1720 CORE, p. 41 */
/* ( x1 -- x2 ) */
static vminstr_p
x_invert(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = ~PICK(sp, 0);
    return ip;
}


/* LSHIFT		6.1.1805 CORE, p. 42 */
/* ( x1 u -- x ) */
static vminstr_p
x_lshift(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) << PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* MAX			6.1.1870 CORE, p. 42 */
/* ( n1 n2 -- n ) */
static vminstr_p
x_max(vminstr_p ip, vmstate_p vm, addr_ft ignore)
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


/* MIN			6.1.1880 CORE, p. 42 */
/* ( n1 n2 -- n ) */
static vminstr_p
x_min(vminstr_p ip, vmstate_p vm, addr_ft ignore)
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


/* NEGATE		6.1.1910 CORE, p. 43 */
/* ( x1 -- x2 ) */
static vminstr_p
x_negate(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = -PICK(sp, 0);
    return ip;
}


/* OR			6.1.1980 CORE, p. 43 */
/* ( x1 x2 -- x ) */
static vminstr_p
x_or(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) | PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* RSHIFT		6.1.2162 CORE, p. 45 */
/* ( x1 u -- x ) */
static vminstr_p
x_rshift(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) >> PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* U<			6.1.2340 CORE, p. 47 */
/* ( u1 u2 -- flag ) */
static vminstr_p
x_u_less(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = -(PICK(sp, 1) < PICK(sp, 0));
    SET_SP(vm, sp, 1);
    return ip;
}


/* XOR			6.1.2490 CORE, p. 49 */
/* ( x1 x2 -- x ) */
static vminstr_p
x_xor(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) ^ PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* 0<> "zero-not-equals"	6.2.0260 CORE EXT, p. 49 */
/* ( x -- flag ) */
static vminstr_p
x_zero_not_equals(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = -((snumber_ft) PICK(sp, 0) != 0);
    return ip;
}


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
    { define_name, "U<",	x_u_less },
    { define_name, "XOR",	x_xor },
    { define_name, "0<>",	x_zero_not_equals },
    { NULL }
};

#if 0
    0>                    6.2.0280 CORE EXT               51
    <>                    6.2.0500 CORE EXT               52
    U>                    6.2.2350 CORE EXT               59
    WITHIN                6.2.2440 CORE EXT               60
#endif
