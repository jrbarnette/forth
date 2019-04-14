/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "forth.h"
#include "dictionary.h"

/*
 * meta.c - Forth primitives to enable meta-interpret and
 *   meta-compilation.
 */


static xt_ft
meta_lookup(vmstate_p vm, vminstr_p ip)
{
    name_p name = lookup(vm, (c_addr_ft) ip->id, strlen(ip->id));
    if (name == NULL) {
	/*
	 * XXX ptrdiff_t wants %ld for 64-bit systems, but this
	 * could cause warnings on 32-bit systems.
	 */
	fprintf(stderr, "name '%s' not found at offset %ld\n",
		ip->id, ip - initialize_forth);
	abort();
    }
    return NAME_XT(name);
}


/* -------------------------------------------------------------- */

vminstr_p
meta_interpret(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    while (ip->id != NULL) {
	execute(vm, meta_lookup(vm, ip));
	ip++;
    }
    return ip + 1;
}


vminstr_p
meta_compile(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    while (ip->id != NULL) {
	COMPILE(vm, meta_lookup(vm, ip));
	ip++;
    }
    return ip + 1;
}


vminstr_p
i_lookup(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    PUSH(vm, meta_lookup(vm, ip));
    return ip + 1;
}


void
interpret_lines(vmstate_p vm, char **lines)
{
    char id[] = "EVALUATE";
    name_p name = lookup(vm, (c_addr_ft) id, sizeof (id) - 1);
    assert(name != NULL);
    xt_ft xt = NAME_XT(name);

    volatile int	throwcode;
    if ((throwcode = CATCH(vm)) != 0) {
	handle_exception(throwcode, vm, NULL);
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
quit(vmstate_p vm, char *filename)
{
    char id[] = "QUIT";
    name_p name = lookup(vm, (c_addr_ft) id, sizeof (id) - 1);
    assert(name != NULL);
    xt_ft xt = NAME_XT(name);

    volatile int	throwcode;
    while ((throwcode = CATCH(vm)) != 0) {
        handle_exception(throwcode, vm, filename);
        if (filename != NULL) {
            return EXIT_FAILURE;
        }
    }
    execute(vm, xt);
    return EXIT_SUCCESS;
}


int
interpret_file(vmstate_p vm, char *filename)
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
