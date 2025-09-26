#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>

#include "bench.h"

/* ----------------  ---------------- */

typedef void (*vminstr_fn)(void);

extern void x_exit(void);
extern void do_colon(void);
extern void do_literal(void);
extern void x_dup(void);
extern void x_swap(void);
extern void x_drop(void);
extern void x_minus(void);
extern void x_zequal(void);
extern void do_skip(void);
extern void do_zskip(void);
extern void do_variable(void);
extern void x_fetch(void);
extern void x_store(void);

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

    ninstr = execute(&vm_state, entrypoint);

    assert(vm_state.rsp == vm_state.rstack + RSTACK_SIZE);
    assert(vm_state.sp == vm_state.stack + STACK_SIZE - 1);
    assert(*vm_state.sp == 0);

    return ninstr;
}
