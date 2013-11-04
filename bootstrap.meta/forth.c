/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

/* ISO/IEC standard */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* IEEE 1003 (POSIX) */
#include <unistd.h>

#include "forth.h"

/*
 * main.c - Initialization and main() for bootstrap Forth
 *   interpreter.
 */


int
main(int argc, char *argv[])
{
    struct vmstate	vmstate;

    init_forth(&vmstate);
    interpret(&vmstate, "DUMP-DICTIONARY");

    return EXIT_SUCCESS;
}
