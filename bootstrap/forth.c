/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include "forth.h"

#include <assert.h>
#include <errno.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
handle_exception(int throwcode, vmstate_p vm, char *filename)
{
    char *excdesc = forth_exceptions[-throwcode-1];

    if (filename != NULL) {
	fprintf(stderr, "error in %s:%lu col %lu\n",
		filename, DICT.lineno, DICT.to_in + 1);
    }
    if (excdesc != NULL) {
	CLEAR_RSTACK(vm);
	CLEAR_STACK(vm);
	fprintf(stderr, "%s\n", excdesc);
    }
}


static vminstr_p
interpret_defs(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    defn_dt *dp = (defn_dt *) ip->ptr;

    while (dp->fn != NULL) {
	dp->fn(vm, dp);
	dp++;
    }

    return ip + 1;
}


#define DEFS(defns)	X(interpret_defs) { .ptr = defns },

static
DIRECT_FORTH(initialize) // {
    DEFS(dictionary_defns)
    DEFS(stackops_defns)
    DEFS(arithops_defns)
    DEFS(memops_defns)
    DEFS(multops_defns)
    DEFS(names_defns)
    DEFS(control_defns)
    DEFS(termio_defns)
    DEFS(interpret_defns)
    DEFS(fileops_defns)
END_DIRECT // }


static void
init_forth(vmstate_p vm)
{
    CLEAR_STACK(vm);
    CLEAR_RSTACK(vm);

    volatile int	throwcode;
    if ((throwcode = setjmp(vm->interp_loop)) != 0) {
	handle_exception(throwcode, vm, NULL);
	abort();
    }

    RPUSH(vm, NULL);
    vminstr_p ip = initialize;
    while (ip != NULL) {
	ip = ip->handler(ip + 1, vm, NULL);
    }

    assert(EMPTY(vm));
    assert(REMPTY(vm));
}


struct options forth_options;

int
main(int argc, char *argv[])
{
    struct vmstate	vmstate;
    volatile int	throwcode;

    process_args(argc, argv, &forth_options);
    init_forth(&vmstate);

    if ((throwcode = setjmp(vmstate.interp_loop)) == 0) {
	interpret_string(&vmstate, initialize_forth);
    } else {
	handle_exception(throwcode, &vmstate, NULL);
    }

    if (forth_options.startup_file != NULL) {
	bool saved_interactive = forth_options.is_interactive;
	forth_options.is_interactive = false;
	FILE *startup = fopen(forth_options.startup_file, "r");
	if (startup != NULL) {
	    if ((throwcode = setjmp(vmstate.interp_loop)) == 0) {
		quit(&vmstate, startup);
	    } else {
		handle_exception(throwcode, &vmstate,
				 forth_options.startup_file);
	    }
	} else {
	    fprintf(stderr,
		    "Can't open startup file %s for reading: %s\n",
		    forth_options.startup_file,
		    strerror(errno));
	}
	(void) fclose(startup);
	forth_options.is_interactive = saved_interactive;
    }

    if (!forth_options.argc) {
	while ((throwcode = setjmp(vmstate.interp_loop)) != 0) {
	    handle_exception(throwcode, &vmstate, NULL);
	}
	quit(&vmstate, stdin);
    } else {
	int i;
	for (i = 0; i < forth_options.argc; i++) {
	    volatile char *filename = forth_options.argv[i];
	    FILE *input = fopen((char *) filename, "r");
	    if (input == NULL) {
		fprintf(stderr,
			"Can't open %s for reading: %s\n",
			(char *) filename, strerror(errno));
		return EXIT_FAILURE;
	    }
	    while ((throwcode = setjmp(vmstate.interp_loop)) != 0) {
		handle_exception(throwcode, &vmstate,
				 (char *) filename);
	    }
	    quit(&vmstate, input);
	    (void) fclose(input);
	}
    }
    return EXIT_SUCCESS;
}
