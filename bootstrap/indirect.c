/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

#include <stddef.h>

#include "forth.h"

/*
 * indirect.c - C utility functions to enable indirect-threaded
 *   interpretation.
 */

// Contains all semantics for the following standard definitions:
//------  ------  ------  ------  ------  ------  ------  ------
// EXECUTE               6.1.1370 CORE
//------  ------  ------  ------  ------  ------  ------  ------

// Contains runtime or other partial semantics for the following:
//------  ------  ------  ------  ------  ------  ------  ------
// :                     6.1.0450 CORE
// CONSTANT              6.1.0950 CORE
// CREATE                6.1.1000 CORE
// VARIABLE              6.1.2410 CORE
//------  ------  ------  ------  ------  ------  ------  ------


void
execute(vmstate_p vm, xt_ft entry_xt)
{
    vminstr_p ip = entry_xt->handler(NULL, vm, entry_xt[1].arg);

    while (ip != NULL) {
	xt_ft xtok = ip->xtok;
	ip = xtok->handler(ip + 1, vm, xtok[1].arg);
    }
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
