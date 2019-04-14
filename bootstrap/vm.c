/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

#include "forth.h"

/*
 * vm.c - Forth primitives and other C code for inner VM operation.
 */

// Contains all semantics for the following standard definitions:
//------  ------  ------  ------  ------  ------  ------  ------
// EXECUTE               6.1.1370 CORE
// EXIT                  6.1.1380 CORE
//
// THROW               9.6.1.2275 EXCEPTION
//------  ------  ------  ------  ------  ------  ------  ------

// Contains runtime or other partial semantics for the following:
//------  ------  ------  ------  ------  ------  ------  ------
// :                     6.1.0450 CORE
// CONSTANT              6.1.0950 CORE
// CREATE                6.1.1000 CORE
// LITERAL               6.1.1780 CORE
// S"                    6.1.2165 CORE
// VARIABLE              6.1.2410 CORE
//
// C"                    6.2.0855 CORE EXT
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


/* ( -- c-addr ) runtime semantics */
vminstr_p
do_c_quote(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft len = ip->cdata[0];

    CHECK_PUSH(vm, 1);
    PUSH(vm, &ip->cdata[0]);
    return (vminstr_p) ((cell_ft) ip + XALIGNED(len + 1));
}


/* ( R: -- nest-sys ) initiation semantics */
vminstr_p
do_colon(vminstr_p ip, vmstate_p vm, vmarg_p newip)
{
    CHECK_RPUSH(vm, 1);
    RPUSH(vm, (cell_ft)ip);
    return newip->vminstrs;
}


/* ( -- x ) name execution semantics */
vminstr_p
do_constant(vminstr_p ip, vmstate_p vm, vmarg_p data_ptr)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, data_ptr->cell);
    return ip;
}


/* ( -- a-addr ) name execution semantics */
vminstr_p
do_create(vminstr_p ip, vmstate_p vm, vmarg_p data_ptr)
{
    vminstr_p does_ptr = data_ptr[0].ip;
    addr_ft body = data_ptr[1].data;

    CHECK_PUSH(vm, 1);
    CHECK_RPUSH(vm, 1);
    PUSH(vm, (cell_ft) body);
    RPUSH(vm, ip);
    return does_ptr;
}


/* ( -- a-addr ) name execution semantics */
vminstr_p
do_variable(vminstr_p ip, vmstate_p vm, vmarg_p var_addr)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) var_addr);
    return ip;
}


/* ( i*x xt -- i*j ) */
vminstr_p
x_execute(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    xt_ft xtok;

    CHECK_POP(vm, 1);
    xtok = (xt_ft)POP(vm);
    return xtok->handler(ip, vm, xtok[1].arg);
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
