/*
 * Copyright 2009, by J. Richard Barnette
 */

#include "forth.h"

/*
 * arithops.c - Standard Forth words for single cell arithmetic and
 *   logical operations.
 */


/* + "plus" 		6.1.0120 CORE, p. 26 */
/* ( x1 x2 -- x ) */
static cell_ft
x_plus(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return POP(vm) + tos;
}


/* - "minus" 		6.1.0160 CORE, p. 27 */
/* ( x1 x2 -- x ) */
static cell_ft
x_minus(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return POP(vm) - tos;
}


/* 0< "zero-less"	6.1.0250 CORE, p. 28 */
/* ( n -- flag ) */
static cell_ft
x_zero_less(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    return -((snumber_ft) tos < 0);
}


/* 0= "zero-quals"	6.1.0270 CORE, p. 28 */
/* ( x -- flag ) */
static cell_ft
x_zero_equals(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    return -(tos == 0);
}


/* 1+ "one-plus"	6.1.0290 CORE, p. 28 */
/* ( n1 -- n2 ) */
static cell_ft
x_one_plus(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    return tos + 1;
}


/* 1- "one-minus"	6.1.0300 CORE, p. 29 */
/* ( n1 -- n2 ) */
static cell_ft
x_one_minus(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    return tos - 1;
}


/* 2* "two-star"	6.1.0320 CORE, p. 29 */
/* ( n1 -- n2 ) */
static cell_ft
x_two_star(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    return (snumber_ft) tos << 1;
}


/* 2/ "two-slash"	6.1.0330 CORE, p. 29 */
/* ( n1 -- n2 ) */
static cell_ft
x_two_slash(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    return (snumber_ft) tos >> 1;
}


/* < "less-than" 	6.1.0480 CORE, p. 30 */
/* ( n1 n2 -- flag ) */
static cell_ft
x_less_than(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return -((snumber_ft) POP(vm) < (snumber_ft) tos);
}


/* = "equals" 		6.1.0530 CORE, p. 31 */
/* ( n1 n2 -- flag ) */
static cell_ft
x_equals(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return -(POP(vm) == tos);
}


/* > "greater-than" 	6.1.0540 CORE, p. 31 */
/* ( n1 n2 -- flag ) */
static cell_ft
x_greater_than(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return -((snumber_ft) POP(vm) > (snumber_ft) tos);
}


/* ABS			6.1.0690 CORE, p. 32 */
/* ( n -- u ) */
static cell_ft
x_abs(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    return ((snumber_ft) tos >= 0) ? tos : -tos;
}


/* AND 			6.1.0720 CORE, p. 33 */
/* ( x1 x2 -- x ) */
static cell_ft
x_and(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return POP(vm) & tos;
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


/* MAX			6.1.1870 CORE, p. 42 */
/* ( n1 n2 -- n ) */
static cell_ft
x_max(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    snumber_ft s1;
    CHECK_POP(vm, 2);
    s1 = (snumber_ft) POP(vm);
    return (s1 > (snumber_ft) tos) ? (cell_ft) s1 : tos;
}


/* MIN			6.1.1880 CORE, p. 42 */
/* ( n1 n2 -- n ) */
static cell_ft
x_min(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    snumber_ft s1;
    CHECK_POP(vm, 2);
    s1 = (snumber_ft) POP(vm);
    return (s1 < (snumber_ft) tos) ? (cell_ft) s1 : tos;
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
    return POP(vm) | tos;
}


/* RSHIFT		6.1.2162 CORE, p. 45 */
/* ( x1 u -- x ) */
static cell_ft
x_rshift(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return POP(vm) >> tos;
}


/* U<			6.1.2340 CORE, p. 47 */
/* ( u1 u2 -- flag ) */
static cell_ft
x_u_less(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return -(POP(vm) < tos);
}


/* XOR			6.1.2490 CORE, p. 49 */
/* ( x1 x2 -- x ) */
static cell_ft
x_xor(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return POP(vm) ^ tos;
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
    { NULL }
};

#if 0
    0<>                   6.2.0260 CORE EXT               51
    0>                    6.2.0280 CORE EXT               51
    <>                    6.2.0500 CORE EXT               52
    U>                    6.2.2350 CORE EXT               59
    WITHIN                6.2.2440 CORE EXT               60
#endif
