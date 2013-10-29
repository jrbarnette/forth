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

    name_p np;
    for (np = DICT.forth_wordlist; np != NULL; np = np->prev) {
	char name[NAME_MAX_LENGTH+1];
	size_t len = NAME_LENGTH(np);
	(void) memcpy(name, np->ident, len);
	name[len] = '\0';
	(void) printf("name: %p  %p %s\n",
			np, NAME_XT(np)->arg[0].xtok, name);
    }
    return EXIT_SUCCESS;
}
