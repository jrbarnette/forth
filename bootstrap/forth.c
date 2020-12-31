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
#include "names.h"
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
interpret_lines(vmstate_ft *vm, char **lines)
{
    char id[] = "EVALUATE";
    name_ft name = lookup(vm, (c_addr_ft) id, sizeof (id) - 1);
    assert(name != NULL);
    xt_ft xt = NAME_XT(name);

    int throwcode;
    if ((throwcode = CATCH(vm)) != 0) {
	report_exception(throwcode, vm, NULL);
        return;
    }

    while (*lines != NULL) {
        char *s = *lines++;
	PUSH(vm, (cell_ft) s);
	PUSH(vm, (cell_ft) strlen(s));
	execute(vm, xt);
    }

    char getstate[] = "STATE @";
    PUSH(vm, getstate);
    PUSH(vm, sizeof (getstate) - 1);
    execute(vm, xt);
    cell_ft state = POP(vm);
    assert(state == 0);
    assert(EMPTY(vm));
    assert(REMPTY(vm));
}


static int
quit(vmstate_ft *vm, char *filename)
{
    char id[] = "QUIT";
    name_ft name = lookup(vm, (c_addr_ft) id, sizeof (id) - 1);
    assert(name != NULL);
    xt_ft xt = NAME_XT(name);

    int throwcode;
    if ((throwcode = CATCH(vm)) != 0) {
        report_exception(throwcode, vm, filename);
	fprintf(stderr, "QUIT failed to handle an exception\n");
	return EXIT_FAILURE;
    }
    execute(vm, xt);
    return EXIT_SUCCESS;
}


static int
interpret_file(vmstate_ft *vm, char *filename)
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

    int exit_status = quit(vm, filename);

    if (filename != NULL) {
        fclose(DICT.input);
    }
    return exit_status;
}


static int
interpret_arguments(vmstate_ft *vm, int argc, char *argv[])
{
    int i;
    for (i = 0; i < argc; i++) {
        int status = interpret_file(vm, argv[i]);
        if (status != EXIT_SUCCESS) {
            return status;
        }
    }
    return EXIT_SUCCESS;
}


static void
initialize_dictionary(vmstate_ft *vm)
{
    direct_execute(vm, initialize_forth);
    assert(EMPTY(vm));
    assert(REMPTY(vm));
    interpret_lines(vm, init_forth_defs);
}


int
main(int argc, char *argv[])
{
    process_args(argc, argv, &forth_options);

    vmstate_ft vmstate;
    initialize_dictionary(&vmstate);

    if (forth_options.startup_file != NULL) {
	bool saved_interactive = forth_options.is_interactive;
	forth_options.is_interactive = false;
        interpret_file(&vmstate, forth_options.startup_file);
	forth_options.is_interactive = saved_interactive;
    }

    if (forth_options.argc > 0) {
	return interpret_arguments(&vmstate,
				   forth_options.argc,
				   forth_options.argv);
    } else {
        if (IS_INTERACTIVE(stdin)) {
            interpret_lines(&vmstate, dictionary_stats);
        }
        return interpret_file(&vmstate, NULL);
    }
}
