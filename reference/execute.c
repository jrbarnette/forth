/*
 * Copyright 2020, by J. Richard Barnette. All Rights Reserved.
 */

#include <stddef.h>

#include "forth.h"

typedef void (*vm_interpreter_fn)(vmstate_p, vminstr_p));


inline vmip_ft
xtcall(xt_ft xtok, vmstate_ft *vm, vmip_ft ip)
{
    return xtok->handler(ip, vm, xtok->arg);
}


void
interpret_direct(vmstate_p vm, vminstr_p ip)
{
    while (ip != NULL) {
	ip = ip->handler(ip + 1, vm, NULL);
    }
}


void
interpret_indirect(vmstate_p vm, vminstr_p ip)
{
    while (ip != NULL) {
	ip = xtcall(ip->xtok, vm, ip + 1);
    }
}


int
execute(vmstate_p vm, vminstr_p ip, vm_interpreter_fn interpret)
{
    int throwcode;
    while ((throwcode = CATCH(vm)) != 0) {
	if (!HAVE_CATCH(vm)) {
	    return throwcode;
	}
	ip = throw_transfer(vm, throwcode);
    }

    interpret(vm, ip);
    return 0;
}


int
execute_direct(vmstate_p vm, vminstr_p ip)
{
    RPUSH(vm, NULL);
    return execute(vm, ip, interpret_direct);
}


int
execute_indirect(vmstate_p vm, xt_ft xtok)
{
    return execute(vm, xtcall(xtok, vm, NULL), interpret_indirect);
}
