/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include "forth.h"

/*
 * arithops.c - Standard Forth words for single cell arithmetic and
 *   logical operations.
 */

/*------  ------  ------  ------  ------  ------  ------  ------
  +                     6.1.0120 CORE                   26
  -                     6.1.0160 CORE                   27
  2*                    6.1.0320 CORE                   29
  2/                    6.1.0330 CORE                   29
  <                     6.1.0480 CORE                   30
  =                     6.1.0530 CORE                   31
  >                     6.1.0540 CORE                   31
  AND                   6.1.0720 CORE                   33
  INVERT                6.1.1720 CORE                   40
  LSHIFT                6.1.1805 CORE                   41
  NEGATE                6.1.1910 CORE                   42
  OR                    6.1.1980 CORE                   42
  RSHIFT                6.1.2162 CORE                   44
  U<                    6.1.2340 CORE                   46
  XOR                   6.1.2490 CORE                   48
  ------  ------  ------  ------  ------  ------  ------  ------
*/


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


/* ( n1 -- n2 ) */
static vminstr_p
x_two_star(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = (snumber_ft) PICK(sp, 0) << 1;
    return ip;
}


/* ( n1 -- n2 ) */
static vminstr_p
x_two_slash(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = (snumber_ft) PICK(sp, 0) >> 1;
    return ip;
}


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


/* ( x1 -- x2 ) */
static vminstr_p
x_invert(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = ~PICK(sp, 0);
    return ip;
}


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


/* ( x1 -- x2 ) */
static vminstr_p
x_negate(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = -PICK(sp, 0);
    return ip;
}


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


defn_dt
arithops_defns[] = {
    { define_name, "+",		x_plus },
    { define_name, "-",		x_minus },
    { define_name, "2*",	x_two_star },
    { define_name, "2/",	x_two_slash },
    { define_name, "<",		x_less_than },
    { define_name, "=",		x_equals },
    { define_name, ">",		x_greater_than },
    { define_name, "AND",	x_and },
    { define_name, "INVERT",	x_invert },
    { define_name, "LSHIFT",	x_lshift },
    { define_name, "NEGATE",	x_negate },
    { define_name, "OR",	x_or },
    { define_name, "RSHIFT",	x_rshift },
    { define_name, "U<",	x_u_less },
    { define_name, "XOR",	x_xor },

    { NULL }
};
