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

typedef xt_ft * (*vminstr_fn)(xt_ft *, vmstate_p, void *);

union vminstr {
    vminstr_fn		handler;
    cell_ft		data[1];
};

/* ----------------  ---------------- */

enum bytecode {
    x_exit,
    do_colon,
    do_literal,
    x_dup,
    x_swap,
    x_drop,
    x_minus,
    x_zequal,
    do_skip,
    do_zskip,
    do_variable,
    x_fetch,
    x_store,
    NUMBYTECODES
}


static unsigned long
execute(vmstate_p vm, xt_ft entry_xt)
{
}

/* ----------------  ---------------- */

static cell_ft *here;
static cell_ft *entrypoint;
static enum bytecode dictionary[256];


void
compile_init(void)
{
    here = dictionary;
    entrypoint = here;
    *here++ = do_colon;
}


void
compile_fini(void)
{
    *here++ = x_exit;
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
