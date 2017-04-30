/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

/* ISO/IEC standard */
#include <stdio.h>
#include <stdbool.h>

/* IEEE 1003 (POSIX) */
#include <unistd.h>

#include "cmdline.h"

/*
 * cmdline.c - Command line parsing and OS-specific initialization.
 */


void
process_args(int argc, char *argv[], struct options *optptr)
{
    char options[] = "if:";
    int c;

    optptr->is_interactive = false;
    optptr->startup_file = NULL;

    while ((c = getopt(argc, argv, options)) != -1) switch (c) {
    case 'i':
	optptr->is_interactive = true;
	break;

    case 'f':
	optptr->startup_file = optarg;
	break;
    }

    optptr->argc = argc - optind;
    optptr->argv = &argv[optind];
}

bool
is_interactive(FILE *fp, struct options *optptr)
{
    return optptr->is_interactive || isatty(fileno(fp)) != 0;
}
