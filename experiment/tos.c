#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>

#include "bench.h"

typedef union vminstr *		xt_ft;

typedef cell_ft (*vminstr_fn)(cell_ft, vmstate_p, void *);

union vminstr {
    vminstr_fn		handler;
    cell_ft		data[1];
};

/* ----------------  ---------------- */

cell_ft
x_exit(cell_ft tos, vmstate_p vm, void *ignore)
{
    vm->ip = (cell_ft *) *vm->rsp++;
    return tos;
}


cell_ft
do_colon(cell_ft tos, vmstate_p vm, void *newip)
{
    *--vm->rsp = (cell_ft) vm->ip;
    vm->ip = (cell_ft *) newip;
    return tos;
}


cell_ft
do_literal(cell_ft tos, vmstate_p vm, void *ignore)
{
    *--vm->sp = tos;
    return (cell_ft) *vm->ip++;
}


cell_ft
x_dup(cell_ft tos, vmstate_p vm, void *ignore)
{
    *--vm->sp = tos;
    return tos;
}


cell_ft
x_swap(cell_ft tos, vmstate_p vm, void *ignore)
{
    cell_ft ntos = *vm->sp;
    *vm->sp = tos;
    return ntos;
}


cell_ft
x_drop(cell_ft tos, vmstate_p vm, void *ignore)
{
    return *vm->sp++;
}


cell_ft
x_minus(cell_ft tos, vmstate_p vm, void *ignore)
{
    return *vm->sp++ - tos;
}


cell_ft
x_zequal(cell_ft tos, vmstate_p vm, void *ignore)
{
    return -(tos == 0);
}


cell_ft
do_skip(cell_ft tos, vmstate_p vm, void *ignore)
{
    vm->ip += (intptr_t) *vm->ip + 1;
    return tos;
}


cell_ft
do_zskip(cell_ft tos, vmstate_p vm, void *ignore)
{
    if (tos == 0)	vm->ip += (intptr_t) *vm->ip + 1;
    else		vm->ip++;
    return *vm->sp++;
}


cell_ft
do_variable(cell_ft tos, vmstate_p vm, void *addr)
{
    *--vm->sp = tos;
    return (cell_ft) addr;
}


cell_ft
x_fetch(cell_ft tos, vmstate_p vm, void *ignore)
{
    return *(cell_ft *)tos;
}


cell_ft
x_store(cell_ft tos, vmstate_p vm, void *ignore)
{
    cell_ft *sp = vm->sp;
    cell_ft data = sp[0];
    *(cell_ft *)tos = data;
    vm->sp = sp + 2;
    return sp[1];
}


unsigned long
execute(vmstate_p vm, cell_ft *entry_xt)
{
    unsigned long ninstr = 0;
    xt_ft xtok = (xt_ft) entry_xt;
    cell_ft tos;

    tos = xtok->handler(vm->stack[STACK_SIZE-1], vm, xtok[1].data);

    while (vm->ip != NULL) {
	ninstr++;
	xtok = (xt_ft) *vm->ip++;
	tos = xtok->handler(tos, vm, xtok[1].data);
    }

    vm->stack[STACK_SIZE-1] = tos;
    return ninstr;
}
