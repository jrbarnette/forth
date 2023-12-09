/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <stddef.h>
#include <string.h>

#include "forth.h"

/*
 * memops.c - Standard Forth words for memory fetch/store, and
 *   pointer arithmetic operations.
 */

/*------  ------  ------  ------  ------  ------  ------  ------
 * !                     6.1.0010 CORE                   25
 * @                     6.1.0650 CORE                   32
 * C!                    6.1.0850 CORE                   34
 * C@                    6.1.0870 CORE                   34
 * FILL                  6.1.1540 CORE                   39
 * MOVE                  6.1.1900 CORE                   42
 *------  ------  ------  ------  ------  ------  ------  ------
 */


/* ( x a-addr -- ) */
PRIM_HDLR(x_store)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    *(cell_ft *)PICK(sp, 0) = PICK(sp, 1);
    SET_SP(vm, sp, 2);
    return ip;
}


/* ( a-addr -- x ) */
PRIM_HDLR(x_fetch)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = *(cell_ft *)PICK(sp, 0);
    return ip;
}


/* ( char c-addr -- ) */
PRIM_HDLR(x_c_store)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    *(char_ft *)PICK(sp, 0) = PICK(sp, 1);
    SET_SP(vm, sp, 2);
    return ip;
}


/* ( c-addr -- char ) */
PRIM_HDLR(x_c_fetch)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = *(char_ft *)PICK(sp, 0);
    return ip;
}


/* ( c-addr u char -- ) */
PRIM_HDLR(x_fill)
{
    char *dst;
    size_t len;
    char c;

    CHECK_POP(vm, 3);
    c = (char) POP(vm);
    len = (size_t) POP(vm);
    dst = (char *) POP(vm);
    (void) memset(dst, c, len);

    return ip;
}


/* ( addr1 addr2 u -- ) */
PRIM_HDLR(x_move)
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
