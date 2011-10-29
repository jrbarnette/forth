/*
 * Copyright 2011, by J. Richard Barnette
 */

/* ISO/IEC standard */
#include <stdbool.h>

/* IEEE 1003 (POSIX) */
#include <unistd.h>

#include "forth.h"

/*
 * cmdline.c - Command line parsing and OS-specific initialization.
 */

void
process_args(int argc, char *argv[], struct options *optptr)
{
    char options[] = "if:";
    int c;

    optptr->is_interactive = (isatty(fileno(stdin)) != 0);
    optptr->startup_file = NULL;

    while ((c = getopt(argc, argv, options)) != -1) switch (c) {
    case 'i':
	optptr->is_interactive = true;
	break;

    case 'f':
	optptr->startup_file = optarg;
	break;
    }
}
