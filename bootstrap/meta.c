/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "forth.h"
#include "dictionary.h"

/*
 * meta.c - Forth primitives to enable meta-interpret and
 *   meta-compilation.
 */


static void
execute(vmstate_p vm, xt_ft entry_xt)
{
    vminstr_p ip = entry_xt->handler(NULL, vm, entry_xt[1].arg);

    while (ip != NULL) {
	xt_ft xtok = ip->xtok;
	ip = xtok->handler(ip + 1, vm, xtok[1].arg);
    }
}


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


void
quit(vmstate_p vm, FILE *input)
{
    char id[] = "QUIT";
    name_p name = lookup(vm, (c_addr_ft) id, sizeof (id) - 1);
    assert(name != NULL);
    xt_ft xt = NAME_XT(name);
    DICT.lineno = 0;
    DICT.input = input;
    execute(vm, xt);
}
