/*
 * Copyright 2011, by J. Richard Barnette
 */

#include <string.h>

#include "forth.h"

/*
 * memops.c - Standard Forth words for memory fetch/store, and
 *   pointer arithmetic operations.
 */


/* ! "store"		6.1.0010 CORE, p. 25 */
/* ( x a-addr -- ) */
static vminstr_p
x_store(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    *(cell_ft *)PICK(sp, 0) = PICK(sp, 1);
    SET_SP(vm, sp, 2);
    return ip;
}


/* @ "fetch" 		6.1.0650 CORE, p. 32 */
/* ( a-addr -- x ) */
static vminstr_p
x_fetch(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = *(cell_ft *)PICK(sp, 0);
    return ip;
}


/* C! "c-store" 	6.1.0850 CORE, p. 34 */
/* ( char c-addr -- ) */
static vminstr_p
x_c_store(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    *(char_ft *)PICK(sp, 0) = PICK(sp, 1);
    SET_SP(vm, sp, 2);
    return ip;
}


/* C@ "c-fetch" 	6.1.0870 CORE, p. 34 */
/* ( c-addr -- char ) */
static vminstr_p
x_c_fetch(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) = *(char_ft *)PICK(sp, 0);
    return ip;
}

/* MOVE			6.1.1900 CORE, p. 42 */
/* ( addr1 addr2 u -- ) */
static vminstr_p
x_move(vminstr_p ip, vmstate_p vm, addr_ft ignore)
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

#if 0
    +!                    6.1.0130 CORE                   27
    2!                    6.1.0310 CORE                   29
    2@                    6.1.0350 CORE                   29
    ALIGNED               6.1.0706 CORE                   33
    CELL+                 6.1.0880 CORE                   35
    CELLS                 6.1.0890 CORE                   35
    CHAR+                 6.1.0897 CORE                   35
    CHARS                 6.1.0898 CORE                   35
    FILL                  6.1.1540 CORE                   39
    ERASE                 6.2.1350 CORE EXT               55
#endif
