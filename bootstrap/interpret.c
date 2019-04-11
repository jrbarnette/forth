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
