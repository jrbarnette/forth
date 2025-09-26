#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>

#include "bench.h"

typedef union vminstr *		xt_ft;

typedef xt_ft * (*vminstr_fn)(xt_ft *, vmstate_p, void *);

union vminstr {
    vminstr_fn		handler;
    cell_ft		data[1];
};

/* ----------------  ---------------- */

xt_ft *
x_exit(xt_ft *ip, vmstate_p vm, void *ignore)
{
    return (xt_ft *) *vm->rsp++;
}


xt_ft *
do_colon(xt_ft *ip, vmstate_p vm, void *newip)
{
    *--vm->rsp = (cell_ft) ip;
    return (xt_ft *) newip;
}


xt_ft *
do_literal(xt_ft *ip, vmstate_p vm, void *ignore)
{
    *--vm->sp = (cell_ft) *ip;
    return ip + 1;
}


xt_ft *
x_dup(xt_ft *ip, vmstate_p vm, void *ignore)
{
    vm->sp--;
    vm->sp[0] = vm->sp[1];
    return ip;
}


xt_ft *
x_swap(xt_ft *ip, vmstate_p vm, void *ignore)
{
    cell_ft *sp = vm->sp;
    cell_ft ntos = sp[1]; sp[1] = sp[0]; sp[0] = ntos;
    return ip;
}


xt_ft *
x_drop(xt_ft *ip, vmstate_p vm, void *ignore)
{
    vm->sp++;
    return ip;
}


xt_ft *
x_minus(xt_ft *ip, vmstate_p vm, void *ignore)
{
    cell_ft diff = vm->sp[1] - vm->sp[0];
    *++vm->sp = diff;
    return ip;
}


xt_ft *
x_zequal(xt_ft *ip, vmstate_p vm, void *ignore)
{
    *vm->sp = -(*vm->sp == 0);
    return ip;
}


xt_ft *
do_skip(xt_ft *ip, vmstate_p vm, void *ignore)
{
    return ip + (intptr_t) *ip + 1;
}


xt_ft *
do_zskip(xt_ft *ip, vmstate_p vm, void *ignore)
{
    cell_ft tos = *vm->sp++;
    if (tos == 0)	return ip + (intptr_t) *ip + 1;
    else		return ip + 1;
}


xt_ft *
do_variable(xt_ft *ip, vmstate_p vm, void *addr)
{
    *--vm->sp = (cell_ft) addr;
    return ip;
}


xt_ft *
x_fetch(xt_ft *ip, vmstate_p vm, void *ignore)
{
    *vm->sp = *(cell_ft *)*vm->sp;
    return ip;
}


xt_ft *
x_store(xt_ft *ip, vmstate_p vm, void *ignore)
{
    cell_ft *sp = vm->sp;
    *(cell_ft *)sp[0] = sp[1];
    vm->sp = sp + 2;
    return ip;
}


unsigned long
execute(vmstate_p vm, cell_ft *entry_xt)
{
    unsigned long ninstr = 0;
    xt_ft xtok = (xt_ft) entry_xt;
    xt_ft *ip;

    ip = xtok->handler((xt_ft *) vm->ip, vm, xtok[1].data);

    while (ip != NULL) {
	ninstr++;
	xt_ft xtok = *ip++;
	ip = xtok->handler(ip, vm, xtok[1].data);
    }

    return ninstr;
}
