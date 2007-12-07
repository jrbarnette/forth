/*
 * Copyright 2007, by J. Richard Barnette
 */

#include <stdio.h>
#include <stdlib.h>

#include "forth.h"
#include "vm.h"
#include "dictionary.h"

/* -------------------------------------------------------------- */
static void
interpret(vmstate_p vm, xt_ft entry_xt)
{
    static vmstate_p save_vm;
    register cell_ft tos;

    CLEAR_STACK(vm);
    CLEAR_RSTACK(vm);
    vm->ip = 0;

    if (setjmp(vm->interp_loop) == 0) {
	save_vm = vm;
	tos = entry_xt->handler(0, entry_xt->data, vm);
    } else {
	vm = save_vm;
	tos = POP(vm);
    }

    if (vm->ip == NULL)
	return;

    for (;;) {
	xt_ft xtok = *vm->ip++;
	tos = xtok->handler(tos, xtok->data, vm);
    }
}

/* -------------------------------------------------------------- */
/*  */
static cell_ft
x_colon(cell_ft tos, addr_ft newip, vmstate_p vm)
{
    CHECK_RPUSH(vm, 1);
    RPUSH(vm, vm->ip);
    vm->ip = (xt_ft *) newip;
    return tos;
}

/* -------------------------------------------------------------- */
/* ( ... xt -- ... ) */
static cell_ft
execute(cell_ft tos, addr_ft ignore, vmstate_p vm)
{
    xt_ft	xtok = (xt_ft) tos;

    CHECK_UNDERFLOW(vm, 1);
    return xtok->handler(POP(vm), xtok->data, vm);
}

/* -------------------------------------------------------------- */
void
overflow(vmstate_p vm)
{
    fprintf(stderr, "overflow\n");
    abort();	/* XXX -3 THROW */
    /*longjmp(vm->interp_loop, 1);*/
}

void
underflow(vmstate_p vm)
{
    fprintf(stderr, "underflow\n");
    abort();	/* XXX -4 THROW */
    /*longjmp(vm->interp_loop, 1);*/
}

/* -------------------------------------------------------------- */
static struct vmstate vmstate;

int
main(int argc, char *argv[])
{
    /* initialize */

    interpret(&vmstate, lookup("QUIT"));

    return EXIT_SUCCESS;
}
