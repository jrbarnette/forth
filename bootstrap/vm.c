/*
 * Copyright 2007, by J. Richard Barnette
 */

#include <stdio.h>
#include <stdlib.h>

#include "forth.h"

void
interpret(vmstate_p vm, xt_ft entry_xt)
{
    static vmstate_p save_vm;
    register cell_ft tos;

    CLEAR_STACK(vm);
    CLEAR_RSTACK(vm);
    vm->ip = NULL;

    if (setjmp(vm->interp_loop) == 0) {
	save_vm = vm;
	tos = entry_xt->handler(0, vm, entry_xt->data);
    } else {
	vm = save_vm;
	tos = POP(vm);
    }

    if (vm->ip == NULL)
	return;

    for (;;) {
	xt_ft xtok = *vm->ip++;
	tos = xtok->handler(tos, vm, xtok->data);
    }
}

void
overflow(vmstate_p vm)
{
    (void) fprintf(stderr, "overflow\n");
    abort();	/* XXX -3 THROW */
    /*longjmp(vm->interp_loop, 1);*/
}

void
underflow(vmstate_p vm)
{
    (void) fprintf(stderr, "underflow\n");
    abort();	/* XXX -4 THROW */
    /*longjmp(vm->interp_loop, 1);*/
}

/* -------------------------------------------------------------- */

/* EXECUTE ( ... xt -- ... ) */
static cell_ft
execute(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    xt_ft	xtok = (xt_ft) tos;

    CHECK_POP(vm, 1);
    return xtok->handler(POP(vm), vm, xtok->data);
}
