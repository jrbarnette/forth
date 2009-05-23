/*
 * Copyright 2009, by J. Richard Barnette
 */

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

#include "forth.h"

/*
 * main.c - Initialization and main() for bootstrap Forth
 *   interpreter.
 */


static
char *forth_exceptions[] =
{
/*  -1 */	NULL,	/* ABORT */
/*  -2 */	NULL,	/* ABORT" */
/*  -3 */	"stack overflow",
/*  -4 */	"stack underflow",
/*  -5 */	"return stack overflow",
/*  -6 */	"return stack underflow",
/*  -7 */	"do-loops nested too deeply during execution",
/*  -8 */	"dictionary overflow",
/*  -9 */	"invalid memory address",
/* -10 */	"division by zero",
/* -11 */	"result out of range",
/* -12 */	"argument type mismatch",
/* -13 */	"undefined word",
/* -14 */	"interpreting a compile-only word",
/* -15 */	"invalid FORGET",
/* -16 */	"attempt to use zero-length string as a name",
/* -17 */	"pictured numeric output string overflow",
/* -18 */	"parsed string overflow",
/* -19 */	"definition name too long",
/* -20 */	"write to a read-only location",
/* -21 */	"unsupported operation (e.g., AT-XY on a too-dumb terminal)",
/* -22 */	"control structure mismatch",
/* -23 */	"address alignment exception",
/* -24 */	"invalid numeric argument",
/* -25 */	"return stack imbalance",
/* -26 */	"loop parameters unavailable",
/* -27 */	"invalid recursion",
/* -28 */	"user interrupt",
/* -29 */	"compiler nesting",
/* -30 */	"obsolescent feature",
/* -31 */	">BODY used on non-CREATEd definition",
/* -32 */	"invalid name argument (e.g., TO xxx)",
/* -33 */	"block read exception",
/* -34 */	"block write exception",
/* -35 */	"invalid block number",
/* -36 */	"invalid file position",
/* -37 */	"file I/O exception",
/* -38 */	"non-existent file",
/* -39 */	"unexpected end of file",
/* -40 */	"invalid BASE for floating point conversion",
/* -41 */	"loss of precision",
/* -42 */	"floating-point divide by zero",
/* -43 */	"floating-point result out of range",
/* -44 */	"floating-point stack overflow",
/* -45 */	"floating-point stack underflow",
/* -46 */	"floating-point invalid argument",
/* -47 */	"compilation word list deleted",
/* -48 */	"invalid POSTPONE",
/* -49 */	"search-order overflow",
/* -50 */	"search-order underflow",
/* -51 */	"compilation word list changed",
/* -52 */	"control-flow stack overflow",
/* -53 */	"exception stack overflow",
/* -54 */	"floating-point underflow",
/* -55 */	"floating-point unidentified fault",
/* -56 */	NULL,	/* QUIT */
/* -57 */	"exception in sending or receiving a character",
/* -58 */	"[IF], [ELSE], or [THEN] exception",
};


static void
init_vm(vmstate_p vm)
{
    CLEAR_STACK(vm);
    CLEAR_RSTACK(vm);
    vm->ip = NULL;
}


static void
interpret_defs(vmstate_p vm, defn_dt *dp)
{
    while (dp->fn != NULL) {
	execute(vm, (xt_ft) dp);
	dp++;
    }
}


static void
handle_exception(int throwcode, vmstate_p vm)
{
    char *excdesc = forth_exceptions[-throwcode-1];

    if (excdesc != NULL) {
	CLEAR_RSTACK(vm);
	CLEAR_STACK(vm);
	fprintf(stderr, "%s\n", excdesc);
    } 
}

static xt_ft
init_forth(vmstate_p vm)
{
    volatile int	throwcode;

    if ((throwcode = setjmp(vm->interp_loop)) != 0) {
	handle_exception(throwcode, vm);
	abort();
    }
    init_vm(vm);

    interpret_defs(vm, dictionary_defns);
    interpret_defs(vm, primitive_defns);
    interpret_defs(vm, format_defns);
    interpret_defs(vm, compile_defns);
}


int
main(int argc, char *argv[])
{
    struct vmstate	vmstate;
    volatile int	throwcode;

    init_forth(&vmstate);

    init_vm(&vmstate);
    while ((throwcode = setjmp(vmstate.interp_loop)) != 0) {
	handle_exception(throwcode, &vmstate);
    }
    quit(&vmstate);
    return EXIT_SUCCESS;
}
