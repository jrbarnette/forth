/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>
#include <stddef.h>

#include "forth.h"
#include "dictionary.h"

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

/* Contains runtime or partial semantics for the following:
 *------  ------  ------  ------  ------  ------  ------  ------
 * LITERAL               6.1.1780 CORE
 * S"                    6.1.2165 CORE
 *
 * C"                    6.2.0855 CORE EXT
 *
 * CATCH               9.6.1.0875 EXCEPTION
 *------  ------  ------  ------  ------  ------  ------  ------
 */


static inline vmip_ft
xtcall(xt_ft xtok, vmstate_ft *vm, vmip_ft ip)
{
    return xtok->handler(ip, vm, xtok->arg);
}


int
execute(vmstate_ft *vm, xt_ft entry_xt)
{
    int throwcode;
    if ((throwcode = CATCH(vm)) != 0) {
	report_exception(throwcode, vm, NULL);
	return throwcode;
    }

    vmip_ft ip = xtcall(entry_xt, vm, NULL);

    while (ip != NULL) {
	ip = xtcall(ip->xtok, vm, ip + 1);
    }
    return 0;
}


/* ( i*x xt -- i*j ) */
PRIM_HDLR(x_execute)
{
    CHECK_POP(vm, 1);
    return xtcall((xt_ft) POP(vm), vm, ip);
}


/* ( R: nest-sys -- ) execution semantics */
PRIM_HDLR(x_exit)
{
    CHECK_RPOP(vm, 1);
    return (vmip_ft) RPOP(vm);
}


/* ( x -- x 0 ) ( R: -- ip sp catch-ptr ) */
PRIM_HDLR(do_catch)
{
    /*
     * In throw_transfer() we replace TOS with a new value.  Using
     * CHECK_POP() here ensures TOS will exist when we throw,
     * because having throw_transfer() handle over/underflow would
     * be ... complicated.
     */
    CHECK_POP(vm, 1);
    CHECK_PUSH(vm, 1);
    CHECK_RPUSH(vm, 3);

    RPUSH(vm, (cell_ft) ip);
    RPUSH(vm, (cell_ft) vm->sp);
    RPUSH(vm, (cell_ft) vm->catch_rsp);
    vm->catch_rsp = vm->rsp;
    PUSH(vm, 0);

    return ip;
}


/* ( -- ) ( R: ip sp catch-ptr -- ) */
PRIM_HDLR(drop_catch)
{
    STACKCHECK(vm, vm->rsp != vm->catch_rsp, -25);
    // this should maybe be a system-specific throw code...
    STACKCHECK(vm, EXEMPTY(vm), -6);
    vm->catch_rsp = (sp_ft) RPOP(vm);
    SET_RSP(vm, RSP(vm), 2);

    return ip;
}


PRIM_HDLR(x_throw)
{
    CHECK_POP(vm, 1);
    cell_ft throw_code = POP(vm);
    CHECK(vm, throw_code != 0, throw_code);
    return ip;
}


/* ( i*x -- ) */
PRIM_HDLR(x_clear)
{
    CLEAR_STACK(vm);
    return ip;
}


/* ( R: i*x -- ) */
PRIM_HDLR(x_rclear)
{
    CLEAR_RSTACK(vm);
    RPUSH(vm, 0);
    return ip;
}


/* ( -- x ) runtime semantics */
PRIM_HDLR(do_literal)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, ip->cell);
    return ip + 1;
}


/* ( -- c-addr u ) runtime semantics */
PRIM_HDLR(do_s_quote)
{
    CHECK_PUSH(vm, 2);
    cell_ft len = ip->cell;
    PUSH(vm, ip + 1);
    PUSH(vm, len);
    return (vmip_ft) (ip->data + XALIGNED(len + CELL_SIZE));
}


/* ( -- c-addr ) runtime semantics */
PRIM_HDLR(do_c_quote)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, ip);
    return (vmip_ft) (ip->data + XALIGNED(ip->cdata[0] + 1));
}
