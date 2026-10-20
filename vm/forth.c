/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

/* ISO/IEC standard */
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* IEEE 1003 (POSIX) */
#include <time.h>

#include "cforth.h"
#include "cmdline.h"
#include "terminal.h"

/*
 * forth.c - Initialization and main() for bootstrap Forth interpreter.
 */


static char *dictionary_stats[] = {
    "allot-bounds                            ( unused+1 -inuse )",
    "negate swap 1- 2dup +                   ( inuse unused total )",
    "swap rot                                ( total unused inuse )",
    ".( dictionary: ) . .( in use / ) . .( free / ) . .( total ) cr",
    NULL,
};


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

#define NEXCEPTIONS	((sizeof (forth_exceptions)) \
			    / (sizeof (forth_exceptions[0])))


static bool
interpret_xt(vmcodeptr_ft xt, struct fargs *args)
{
    int throwcode = forth_execute(xt, args);
    if (throwcode == 0) {
	return true;
    }

    if (termconfig.filename != NULL) {
	fprintf(stderr, "error in %s, line %zu\n",
		termconfig.filename, termconfig.lineno);
    // `termconfig.input == NULL` means "in initialize_dictionary()"
    } else if (termconfig.input != NULL &&
	       !termconfig.is_interactive) {
	fprintf(stderr, "error at line %zu\n", termconfig.lineno);
    }

    char *excdesc = NULL;
    if (~throwcode < NEXCEPTIONS) {
	excdesc = forth_exceptions[~throwcode];
    }

    if (excdesc != NULL) {
	fprintf(stderr, "%s\n", excdesc);
    } else {
	fprintf(stderr, "exception %d\n", throwcode);
    }
    return false;
}


static void
interpret_lines(vmcodeptr_ft eval, char **lines)
{
    FARGS0(args);
    while (*lines != NULL) {
	assert(args.depth <= FARGS_LEN - 2);
	char *s = *lines++;
	args.stack[args.depth] = (cell_ft) s;
	args.stack[args.depth + 1] = strlen(s);
	args.depth += 2;
	if (!interpret_xt(eval, &args)) {
	    return;
	}
    }

    assert(args.depth == 0);
    char getstate[] = "STATE @";
    args.stack[0] = (cell_ft) getstate;
    args.stack[1] = sizeof (getstate) - 1;
    args.depth = 2;
    if (!interpret_xt(eval, &args)) {
	return;
    }
    assert(args.depth == 1);
    assert(args.stack[0] == 0);
}


static int
quit(vmcodeptr_ft eval)
{
    char quit_cmd[] = "QUIT";
    FARGS2(args, quit_cmd, sizeof (quit_cmd) - 1);
    if (!interpret_xt(eval, &args)) {
	fprintf(stderr, "QUIT failed to handle an exception\n");
	return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


static int
interpret_file(vmcodeptr_ft eval, char *filename)
{
    if (!term_open(filename)) {
	fprintf(stderr,
		"Can't open %s for reading: %s\n",
		(char *) filename, strerror(errno));
	return EXIT_FAILURE;
    }

    int exit_status = quit(eval);

    term_close();

    return exit_status;
}


static int
interpret_arguments(vmcodeptr_ft eval, int argc, char *argv[])
{
    for (int i = 0; i < argc; i++) {
	int status = interpret_file(eval, argv[i]);
	if (status != EXIT_SUCCESS) {
	    return status;
	}
    }
    return EXIT_SUCCESS;
}


static vmcodeptr_ft
initialize_dictionary(void)
{
    FARGS0(args);
    if (!interpret_xt(initialize_forth, &args)) {
	fprintf(stderr, "Exception in initialize_forth\n");
	abort();
    }
    assert(args.depth == 1);
    return (vmcodeptr_ft) args.stack[0];
}


int
main(int argc, char *argv[])
{
    struct options forth_options;
    process_args(argc, argv, &forth_options);

    struct timespec curtime;
    clock_gettime(CLOCK_REALTIME, &curtime);
    long start = 1000000000 * curtime.tv_sec + curtime.tv_nsec;
    vmcodeptr_ft eval = initialize_dictionary();
    clock_gettime(CLOCK_REALTIME, &curtime);
    long end = 1000000000 * curtime.tv_sec + curtime.tv_nsec;

    if (forth_options.startup_file != NULL) {
	interpret_file(eval, forth_options.startup_file);
    }

    int status;
    if (forth_options.argc > 0) {
	status = interpret_arguments(eval,
				     forth_options.argc,
				     forth_options.argv);
    } else {
	term_set_input(stdin, forth_options.is_interactive);
	if (termconfig.is_interactive) {
	    long elapsed = end - start + 50000;
	    fprintf(stderr, "initialization: %ld.%ld ms\n",
		    elapsed / 1000000, (elapsed % 1000000) / 100000);
	    interpret_lines(eval, dictionary_stats);
	}
	status = quit(eval);
    }

#ifdef STACKPROFILE
    stacks_report();
#endif
    return status;
}
