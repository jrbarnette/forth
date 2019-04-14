/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "forth.h"
#include "dictionary.h"
#include "cmdline.h"

/*
 * main.c - Initialization and main() for bootstrap Forth
 *   interpreter.
 */


struct options forth_options;

char *dictionary_stats[] = {
    "unused here forth-wordlist - cell+         ( free inuse )",
    "2dup + rot rot                             ( total free inuse )",
    ".( dictionary: ) . .( in use / ) . .( free / ) . .( total ) cr",
    NULL,
};


static int
interpret_arguments(vmstate_p vm, int argc, char *argv[])
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


int
main(int argc, char *argv[])
{
    struct vmstate	vmstate;

    process_args(argc, argv, &forth_options);
    direct_execute(&vmstate, initialize_forth);
    interpret_lines(&vmstate, init_forth_defs);

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
