/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "forth.h"

/*
 * interpret.c - Outer (interactive) interpreter, and related Forth
 *   words.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// [                     6.1.2500 CORE                   48
// ]                     6.1.2540 CORE                   49
//------  ------  ------  ------  ------  ------  ------  ------


/* ( -- x ) runtime semantics */
vminstr_p
do_literal(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, ip->cell);
    return ip + 1;
}


/* ( -- c-addr u ) runtime semantics */
vminstr_p
do_s_quote(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft len = ip->cell;

    CHECK_PUSH(vm, 2);
    PUSH(vm, ip + 1);
    PUSH(vm, len);
    return (vminstr_p) ((cell_ft) ip + XALIGNED(len + CELL_SIZE));
}


/* ( -- ) compilation semantics */
vminstr_p
x_left_bracket(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    DICT.state = STATE_INTERP;
    return ip;
}


/* ( -- ) */
vminstr_p
x_right_bracket(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    DICT.state = STATE_COMPILE;
    return ip;
}


/* ( -- c-addr ) runtime semantics */
vminstr_p
do_c_quote(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft len = ip->cdata[0];

    CHECK_PUSH(vm, 1);
    PUSH(vm, &ip->cdata[0]);
    return (vminstr_p) ((cell_ft) ip + XALIGNED(len + 1));
}
