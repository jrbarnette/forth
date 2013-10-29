/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <string.h>

#include "forth.h"

/*
 * memops.c - Standard Forth words for memory fetch/store, and
 *   pointer arithmetic operations.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// !                     6.1.0010 CORE                   25
// +!                    6.1.0130 CORE                   27
// 2!                    6.1.0310 CORE                   29
// 2@                    6.1.0350 CORE                   29
// @                     6.1.0650 CORE                   32
// ALIGNED               6.1.0706 CORE                   33
// C!                    6.1.0850 CORE                   34
// C@                    6.1.0870 CORE                   34
// CELL+                 6.1.0880 CORE                   35
// CELLS                 6.1.0890 CORE                   35
// CHAR+                 6.1.0897 CORE                   35
// CHARS                 6.1.0898 CORE                   35
// COUNT                 6.1.0980 CORE                   36
// FILL                  6.1.1540 CORE                   39
// MOVE                  6.1.1900 CORE                   42
//
// ERASE                 6.2.1350 CORE EXT               53
//------  ------  ------  ------  ------  ------  ------  ------


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


DIRECT_FORTH(init_memory_prim) // {
    PRIM("!", 		x_store)
    PRIM("@",		x_fetch)
    PRIM("C!",		x_c_store)
    PRIM("C@",		x_c_fetch)
    PRIM("MOVE",	x_move)
END_DIRECT // }


META_FORTH(init_memory_ops) // {
    XCOLON("ALIGNED") L(CELL_SIZE-1) PLUS L(-CELL_SIZE) AND XSEMICOLON
    XCOLON("CELL+") L(CELL_SIZE) PLUS XSEMICOLON
    XCOLON("CELLS") L(CELL_SHIFT) LSHIFT XSEMICOLON
    XCOLON("CHAR+") L(CHAR_SIZE) PLUS XSEMICOLON
    XCOLON("CHARS") XSEMICOLON		/* XXX assert(CHAR_SIZE == 1) */
END_META // }

// +!                    6.1.0130 CORE                   27
// 2!                    6.1.0310 CORE                   29
// 2@                    6.1.0350 CORE                   29
// C!                    6.1.0850 CORE                   34
// C@                    6.1.0870 CORE                   34
// COUNT                 6.1.0980 CORE                   36
// FILL                  6.1.1540 CORE                   39
// ERASE                 6.2.1350 CORE EXT               53
