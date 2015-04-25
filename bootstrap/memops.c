/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <string.h>

#include "forth.h"

/*
 * memops.c - Standard Forth words for memory fetch/store, and
 *   pointer arithmetic operations.
 */

/*------  ------  ------  ------  ------  ------  ------  ------
  !                     6.1.0010 CORE                   25
  @                     6.1.0650 CORE                   32
  C!                    6.1.0850 CORE                   34
  C@                    6.1.0870 CORE                   34
  MOVE                  6.1.1900 CORE                   42
  ------  ------  ------  ------  ------  ------  ------  ------
*/


/* ( x a-addr -- ) */
vminstr_p
x_store(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    *(cell_ft *)PICK(sp, 0) = PICK(sp, 1);
    SET_SP(vm, sp, 2);
    return ip;
}


/* ( a-addr -- x ) */
vminstr_p
x_fetch(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = *(cell_ft *)PICK(sp, 0);
    return ip;
}


/* ( char c-addr -- ) */
vminstr_p
x_c_store(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    *(char_ft *)PICK(sp, 0) = PICK(sp, 1);
    SET_SP(vm, sp, 2);
    return ip;
}


/* ( c-addr -- char ) */
vminstr_p
x_c_fetch(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = *(char_ft *)PICK(sp, 0);
    return ip;
}


/* ( addr1 addr2 u -- ) */
vminstr_p
x_move(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    char *src, *dst;
    size_t len;

    CHECK_POP(vm, 3);
    len = (size_t) POP(vm);
    dst = (char *) POP(vm);
    src = (char *) POP(vm);
    (void) memmove(dst, src, len);

    return ip;
}


defn_dt
memops_defns[] = {
    { define_name, "!",		x_store },
    { define_name, "@",		x_fetch },
    { define_name, "C!",	x_c_store },
    { define_name, "C@",	x_c_fetch },
    { define_name, "MOVE",	x_move },
    { NULL }
};
