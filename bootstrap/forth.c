/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "forth.h"
#include "dictionary.h"
#include "cmdline.h"

/*
 * forth.c - Initialization and main() for bootstrap Forth
 *   interpreter.
 */


union dict dictionary;

struct options forth_options;

static char *dictionary_stats[] = {
    "allot-bounds swap 1- 2dup -             ( -inuse free -total )",
    "negate swap rot negate                  ( total free inuse )",
    ".( dictionary: ) . .( in use / ) . .( free / ) . .( total ) cr",
    NULL,
};


static void
interpret_lines(xt_ft eval, vmstate_ft *vm, char **lines)
{
    while (*lines != NULL) {
	char *s = *lines++;
	PUSH(vm, (cell_ft) s);
	PUSH(vm, (cell_ft) strlen(s));
	if (execute(vm, eval) != 0) {
	    return;
	}
    }

    char getstate[] = "STATE @";
    PUSH(vm, getstate);
    PUSH(vm, sizeof (getstate) - 1);
    if (execute(vm, eval) != 0) {
	return;
    }
    cell_ft state = POP(vm);
    assert(state == 0);
    assert(EMPTY(vm));
    assert(REMPTY(vm));
}


static int
quit(xt_ft eval, vmstate_ft *vm, char *filename)
{
    char quit_cmd[] = "QUIT";
    PUSH(vm, quit_cmd);
    PUSH(vm, sizeof (quit_cmd) - 1);
    if (execute(vm, eval) != 0) {
	fprintf(stderr, "QUIT failed to handle an exception\n");
	return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


static int
interpret_file(xt_ft eval, vmstate_ft *vm, char *filename)
{
    DICT.lineno = 0;
    if (filename != NULL) {
	FILE *input = fopen(filename, "r");
	if (input == NULL) {
	    fprintf(stderr,
		    "Can't open %s for reading: %s\n",
		    (char *) filename, strerror(errno));
	    return EXIT_FAILURE;
	}
	DICT.input = input;
    } else {
	DICT.input = stdin;
    }

    int exit_status = quit(eval, vm, filename);

    if (filename != NULL) {
	fclose(DICT.input);
    }
    return exit_status;
}


static int
interpret_arguments(xt_ft eval, vmstate_ft *vm, int argc, char *argv[])
{
    int i;
    for (i = 0; i < argc; i++) {
	int status = interpret_file(eval, vm, argv[i]);
	if (status != EXIT_SUCCESS) {
	    return status;
	}
    }
    return EXIT_SUCCESS;
}


static xt_ft
initialize_dictionary(vmstate_ft *vm)
{
    HERE = DICTSPACE_START + sizeof (DICT);
    direct_execute(vm, initialize_forth);
    xt_ft eval = (xt_ft) POP(vm);
    assert(EMPTY(vm));
    assert(REMPTY(vm));
    interpret_lines(eval, vm, init_forth_defs);

    return eval;
}


int
main(int argc, char *argv[])
{
    process_args(argc, argv, &forth_options);

    vmstate_ft vmstate;
    xt_ft eval = initialize_dictionary(&vmstate);

    if (forth_options.startup_file != NULL) {
	bool saved_interactive = forth_options.is_interactive;
	forth_options.is_interactive = false;
	interpret_file(eval, &vmstate, forth_options.startup_file);
	forth_options.is_interactive = saved_interactive;
    }

    if (forth_options.argc > 0) {
	return interpret_arguments(eval, &vmstate,
				   forth_options.argc,
				   forth_options.argv);
    } else {
	if (IS_INTERACTIVE(stdin)) {
	    interpret_lines(eval, &vmstate, dictionary_stats);
	}
	return interpret_file(eval, &vmstate, NULL);
    }
}
