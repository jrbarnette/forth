/*
 * Copyright 2009, by J. Richard Barnette
 */

#include "forth.h"

/*
 * memops.c - Standard Forth words for memory fetch/store, and
 *   pointer arithmetic operations.
 */


/* ! "store"		6.1.0010 CORE, p. 25 */
/* ( x a-addr -- ) */
static cell_ft
x_store(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    *(a_addr_ft)tos = POP(vm);
    return POP(vm);
}


/* @ "fetch" 		6.1.0650 CORE, p. 32 */
/* ( a-addr -- x ) */
static cell_ft
x_fetch(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    return *(a_addr_ft)tos;
}


/* C! "c-store" 	6.1.0850 CORE, p. 34 */
/* ( char c-addr -- ) */
static cell_ft
x_c_store(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    *(c_addr_ft)tos = (char_ft) POP(vm);
    return POP(vm);
}


/* C@ "c-fetch" 	6.1.0870 CORE, p. 34 */
/* ( c-addr -- char ) */
static cell_ft
x_c_fetch(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    return *(c_addr_ft)tos;
}


defn_dt
memops_defns[] = {
    { define_name, "!",		x_store },
    { define_name, "@",		x_fetch },
    { define_name, "C!",	x_c_store },
    { define_name, "C@",	x_c_fetch },
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
    MOVE                  6.1.1900 CORE                   43
    ERASE                 6.2.1350 CORE EXT               55
#endif
