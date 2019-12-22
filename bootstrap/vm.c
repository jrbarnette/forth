/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

#include "forth.h"

/*
 * vm.c - Forth primitives and other C code for inner VM operation.
 */

/* Contains all semantics for the following standard definitions:
 *------  ------  ------  ------  ------  ------  ------  ------
 * EXIT                  6.1.1380 CORE
 *
 * THROW               9.6.1.2275 EXCEPTION
 *------  ------  ------  ------  ------  ------  ------  ------
 */

/* Contains runtime or other partial semantics for the following:
 *------  ------  ------  ------  ------  ------  ------  ------
 * LITERAL               6.1.1780 CORE
 * S"                    6.1.2165 CORE
 *
 * C"                    6.2.0855 CORE EXT
 *------  ------  ------  ------  ------  ------  ------  ------
 */


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


/* ( -- c-addr ) runtime semantics */
vminstr_p
do_c_quote(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft len = ip->cdata[0];

    CHECK_PUSH(vm, 1);
    PUSH(vm, &ip->cdata[0]);
    return (vminstr_p) ((cell_ft) ip + XALIGNED(len + 1));
}


/* ( R: nest-sys -- ) execution semantics */
vminstr_p
x_exit(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_RPOP(vm, 1);
    return (vminstr_p)RPOP(vm);
}


vminstr_p
x_throw(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    cell_ft exc = POP(vm);
    if (exc != 0) {
	THROW(vm, exc);
    }
    return ip;
}


/* ( i*x -- ) */
vminstr_p
x_clear(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CLEAR_STACK(vm);
    return ip;
}


/* ( i*x -- ) */
vminstr_p
x_rclear(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CLEAR_RSTACK(vm);
    return ip;
}
