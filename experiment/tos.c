#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>

#include "bench.h"

#define STACK_SIZE	2048
#define RSTACK_SIZE	64

typedef union vminstr *		xt_ft;
typedef struct vmstate *	vmstate_p;

struct vmstate {
    xt_ft *	ip;
    cell_ft *	sp;
    cell_ft *	rsp;
    cell_ft	stack[STACK_SIZE];
    cell_ft	rstack[RSTACK_SIZE];
};

typedef cell_ft (*vminstr_fn)(cell_ft, vmstate_p, void *);

union vminstr {
    vminstr_fn		handler;
    cell_ft		data[1];
};

/* ----------------  ---------------- */

static cell_ft
x_exit(cell_ft tos, vmstate_p vm, void *ignore)
{
    vm->ip = (xt_ft *) *vm->rsp++;
    return tos;
}


static cell_ft
do_colon(cell_ft tos, vmstate_p vm, void *newip)
{
    *--vm->rsp = (cell_ft) vm->ip;
    vm->ip = (xt_ft *) newip;
    return tos;
}


static cell_ft
do_literal(cell_ft tos, vmstate_p vm, void *ignore)
{
    *--vm->sp = tos;
    return (cell_ft) *vm->ip++;
}


static cell_ft
x_dup(cell_ft tos, vmstate_p vm, void *ignore)
{
    *--vm->sp = tos;
    return tos;
}


static cell_ft
x_swap(cell_ft tos, vmstate_p vm, void *ignore)
{
    cell_ft ntos = *vm->sp;
    *vm->sp = tos;
    return ntos;
}


static cell_ft
x_drop(cell_ft tos, vmstate_p vm, void *ignore)
{
    return *vm->sp++;
}


static cell_ft
x_minus(cell_ft tos, vmstate_p vm, void *ignore)
{
    return *vm->sp++ - tos;
}


static cell_ft
x_zequal(cell_ft tos, vmstate_p vm, void *ignore)
{
    return -(tos == 0);
}


static cell_ft
do_skip(cell_ft tos, vmstate_p vm, void *ignore)
{
    vm->ip += (intptr_t) *vm->ip + 1;
    return tos;
}


static cell_ft
do_zskip(cell_ft tos, vmstate_p vm, void *ignore)
{
    if (tos == 0)	vm->ip += (intptr_t) *vm->ip + 1;
    else		vm->ip++;
    return *vm->sp++;
}


static cell_ft
do_variable(cell_ft tos, vmstate_p vm, void *addr)
{
    *--vm->sp = tos;
    return (cell_ft) addr;
}


static cell_ft
x_fetch(cell_ft tos, vmstate_p vm, void *ignore)
{
    return *(cell_ft *)tos;
}


static cell_ft
x_store(cell_ft tos, vmstate_p vm, void *ignore)
{
    cell_ft *sp = vm->sp;
    cell_ft data = sp[0];
    *(cell_ft *)tos = data;
    vm->sp = sp + 2;
    return sp[1];
}


static unsigned long
execute(vmstate_p vm, xt_ft entry_xt)
{
    unsigned long ninstr = 0;
    cell_ft tos;

    tos = entry_xt->handler(vm->stack[STACK_SIZE-1], vm, entry_xt[1].data);

    while (vm->ip != NULL) {
	ninstr++;
	xt_ft xtok = *vm->ip++;
	tos = xtok->handler(tos, vm, xtok[1].data);
    }

    vm->stack[STACK_SIZE-1] = tos;
    return ninstr;
}

/* ----------------  ---------------- */

static cell_ft *here;
static cell_ft *entrypoint;
static cell_ft dictionary[256];
static label_ft primtable[NPRIMITIVES];


void
compile_init(void)
{
    here = dictionary;
    primtable[prim_exit] = here;
    *here++ = (cell_ft) x_exit;

    primtable[prim_nop] = here;
    *here++ = (cell_ft) do_colon;
    *here++ = (cell_ft) primtable[prim_exit];

    primtable[prim_literal] = here;
    *here++ = (cell_ft) do_literal;

    primtable[prim_dup] = here;
    *here++ = (cell_ft) x_dup;

    primtable[prim_swap] = here;
    *here++ = (cell_ft) x_swap;

    primtable[prim_drop] = here;
    *here++ = (cell_ft) x_drop;

    primtable[prim_drop] = here;
    *here++ = (cell_ft) x_drop;

    primtable[prim_minus] = here;
    *here++ = (cell_ft) x_minus;

    primtable[prim_zequal] = here;
    *here++ = (cell_ft) x_zequal;

    primtable[prim_skip] = here;
    *here++ = (cell_ft) do_skip;

    primtable[prim_zskip] = here;
    *here++ = (cell_ft) do_zskip;

    primtable[prim_addr] = here;
    *here++ = (cell_ft) do_variable;
    here++;

    primtable[prim_fetch] = here;
    *here++ = (cell_ft) x_fetch;

    primtable[prim_store] = here;
    *here++ = (cell_ft) x_store;

    entrypoint = here;
    *here++ = (cell_ft) do_colon;
}


void
compile_fini(void)
{
    *here++ = (cell_ft) primtable[prim_exit];
}


void
compile(enum primitive pc, ...)
{
    va_list args;

    *here++ = (cell_ft) primtable[pc];

    va_start(args, pc);
    switch (pc) {
    case prim_literal:
	*here++ = va_arg(args, cell_ft);
	break;

    case prim_skip:
    case prim_zskip:
	*here = va_arg(args, label_ft) - here - 1;
	here++;
	break;
    default:
	break;
    }
    va_end(args);
}


label_ft
curloc(void)
{
    return here;
}


void
patch(label_ft patchloc)
{
    patchloc[1] = here - patchloc - 2;
}


unsigned long
interpret(void)
{
    unsigned long ninstr;
    struct vmstate vm_state;

    vm_state.ip = NULL;
    vm_state.sp = vm_state.stack + STACK_SIZE;
    vm_state.rsp = vm_state.rstack + RSTACK_SIZE;

    ninstr = execute(&vm_state, (xt_ft) entrypoint);

    assert(vm_state.rsp == vm_state.rstack + RSTACK_SIZE);
    assert(vm_state.sp == vm_state.stack + STACK_SIZE - 1);
    assert(*vm_state.sp == 0);

    return ninstr;
}
