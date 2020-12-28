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


static xt_ft
meta_lookup(vmstate_p vm, vminstr_p ip)
{
    name_p name = lookup(vm, (c_addr_ft) ip->id, strlen(ip->id));
    if (name == NULL) {
	fprintf(stderr, "name '%s' not found at offset %td\n",
		ip->id, ip - initialize_forth);
	abort();
    }
    return NAME_XT(name);
}


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
