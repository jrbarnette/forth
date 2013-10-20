/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include "forth.h"

/*
 * multops.c - Standard Forth words for double cell multiply
 *   and divide operations.
 */

/*
 * N.B. The Forth spellings of star-slash and star-slash-mode can't
 * appear in a comment.  Go ahead, try it.  Make my day.
 */
/*------  ------  ------  ------  ------  ------  ------  ------
  *                     6.1.0090 CORE                   26
  star-slash            6.1.0100 CORE                   26
  star-slash-mod        6.1.0110 CORE                   26
  /                     6.1.0230 CORE                   28
  /MOD                  6.1.0240 CORE                   28
  M*                    6.1.1810 CORE                   41
  MOD                   6.1.1890 CORE                   42
  UM*                   6.1.2360 CORE                   46
  ------  ------  ------  ------  ------  ------  ------  ------
*/


/* ( x1 x2 -- x ) */
static vminstr_p
x_star(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) * PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( n1 n2 -- n ) */
static vminstr_p
x_slash(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    snumber_ft n1 = PICK(sp, 1);
    snumber_ft n2 = PICK(sp, 0);
    PICK(sp, 0) = (cell_ft) (n1 / n2);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( n1 n2 -- n3 n4 ) */
static vminstr_p
x_slash_mod(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    snumber_ft n1 = PICK(sp, 1);
    snumber_ft n2 = PICK(sp, 0);
    PICK(sp, 1) = (cell_ft) (n1 / n2);
    PICK(sp, 0) = (cell_ft) (n1 % n2);
    return ip;
}


/* ( n1 n2 -- n ) */
static vminstr_p
x_mod(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    snumber_ft n1 = PICK(sp, 1);
    snumber_ft n2 = PICK(sp, 0);
    PICK(sp, 0) = (cell_ft) (n1 % n2);
    SET_SP(vm, sp, 1);
    return ip;
}


defn_dt
multops_defns[] = {
    { define_name, "*",		x_star },
    { define_name, "/",		x_slash },
    { define_name, "/MOD",	x_slash_mod },
    { define_name, "MOD",	x_mod },
    { NULL }
};
