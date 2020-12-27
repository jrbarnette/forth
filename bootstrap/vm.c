/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

#include <stddef.h>

#include "forth.h"

/*
 * vm.c - Forth primitives and other C code for inner VM operation.
 */

/* Contains all semantics for the following standard definitions:
 *------  ------  ------  ------  ------  ------  ------  ------
 * EXECUTE               6.1.1370 CORE
 * EXIT                  6.1.1380 CORE
 *
 * THROW               9.6.1.2275 EXCEPTION
 *------  ------  ------  ------  ------  ------  ------  ------
 */

/* Contains runtime semantics for the following:
 *------  ------  ------  ------  ------  ------  ------  ------
 * LITERAL               6.1.1780 CORE
 * S"                    6.1.2165 CORE
 *
 * C"                    6.2.0855 CORE EXT
 *------  ------  ------  ------  ------  ------  ------  ------
 */


void
vm_initialize(vmstate_p vm)
{
    CLEAR_STACK(vm);
    CLEAR_RSTACK(vm);
    vm->catch_handler = NULL;
}


void
execute(vmstate_p vm, xt_ft entry_xt)
{
    vminstr_p ip = entry_xt->handler(NULL, vm, entry_xt->arg);

    while (ip != NULL) {
	xt_ft xtok = ip->xtok;
	ip = xtok->handler(ip + 1, vm, xtok->arg);
    }
}


vminstr_p
throw_transfer(vmstate_p vm, cell_ft throw_code)
{
    if (vm->catch_handler == NULL) {
        THROW(vm, throw_code);
	return NULL;
    }

    vm->rsp = vm->catch_handler;
    cell_ft *rsp = RSP(vm);

    vminstr_p newip = (vminstr_p) PICK(rsp, 2);
    vm->sp = (cell_ft *) PICK(rsp, 1);
    vm->catch_handler = (cell_ft *) PICK(rsp, 0);
    SET_RSP(vm, rsp, 3);

    PICK(SP(vm), 0) = throw_code;

    return newip;
}


/* ( -- 0 ) ( R: -- ip sp catch-ptr ) */
vminstr_p
do_catch(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    CHECK_RPUSH(vm, 3);
    cell_ft *rsp = RSP(vm);
    PICK(rsp, -1) = (cell_ft) ip;
    PICK(rsp, -2) = (cell_ft) vm->sp;
    PICK(rsp, -3) = (cell_ft) vm->catch_handler;
    SET_RSP(vm, rsp, -3);
    vm->catch_handler = vm->rsp;
    PUSH(vm, 0);

    return ip;
}


/* ( -- ) ( R: ip sp catch-ptr -- ) */
vminstr_p
undo_catch(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_RPOP(vm, 3);
    cell_ft *rsp = RSP(vm);
    vm->catch_handler = (cell_ft *) PICK(rsp, 0);
    SET_RSP(vm, rsp, 3);

    return ip;
}


/* ( i*x xt -- i*j ) */
vminstr_p
x_execute(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    xt_ft xtok;

    CHECK_POP(vm, 1);
    xtok = (xt_ft)POP(vm);
    return xtok->handler(ip, vm, xtok->arg);
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
    cell_ft throw_code = POP(vm);
    if (throw_code == 0) {
	return ip;
    }
    return throw_transfer(vm, throw_code);
}


/* ( i*x -- ) */
vminstr_p
x_clear(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CLEAR_STACK(vm);
    return ip;
}


/* ( R: i*x -- ) */
vminstr_p
x_rclear(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CLEAR_RSTACK(vm);
    return ip;
}


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
