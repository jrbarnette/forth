/*
 * Copyright 2007, by J. Richard Barnette
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "forth.h"

/*
 * forth.c - Initialization and main() for bootstrap Forth
 *   interpreter.
 */


static struct vmstate vmstate;

static void
add_defns(defn_dt *dp)
{
    name_p nm;

    while (dp->name != NULL) {
	nm = addname(dp->name, dp->fn);
	if (dp->definer != NULL) {
	    dp->definer(nm, dp->datap);
	}

	dp++;
    }
}

void
init_forth(void)
{
    add_defns(dictionary_defns);
    add_defns(primitive_defns);
    add_defns(compile_defns);
}

void
compile(FILE *in)
{
    c_addr_ft	word;
    name_p	name;

    while ((word = parse(in, ' ')) != NULL) {
	name = lookup(word, strlen(word));
	if (name == NULL) {
	    fprintf(stderr, "not found: %s\n", word);
	    exit(EXIT_FAILURE);
	}
	*(xt_ft *)allot(CELL_SIZE) = NAME_CFA(name);
    }

    name = lookup("EXIT", 4);
    *(xt_ft *)allot(CELL_SIZE) = NAME_CFA(name);
}

int
main(int argc, char *argv[])
{
    name_p	quit;
    xt_ft	testcode;

    init_forth();

    testcode = noname();
    compile(stdin);

    interpret(&vmstate, testcode);

    return EXIT_SUCCESS;
}
