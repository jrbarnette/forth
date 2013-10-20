/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <stdio.h>

#include "forth.h"

/*
 * termio.c - Standard Forth words for user terminal I/O.
 */

/*------  ------  ------  ------  ------  ------  ------  ------
  EMIT                  6.1.1320 CORE                   38
  ------  ------  ------  ------  ------  ------  ------  ------
*/


/* ( x -- ) */
static vminstr_p
x_emit(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    putc((char_ft) POP(vm), stdout);
    return ip;
}


defn_dt
termio_defns[] = {
    { define_name, "EMIT",	x_emit },
    { NULL }
};
