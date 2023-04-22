/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "forth.h"
#include "names.h"
#include "dictionary.h"
#include "meta.h"

/*
 * meta.c - Forth primitives to enable meta-interpret and
 *   meta-compilation.
 */


static xt_ft
meta_lookup(vmstate_ft *vm, vmip_ft ip)
{
    name_ft name = lookup(vm, (c_addr_ft) ip->id, strlen(ip->id));
    if (name == NULL) {
	fprintf(stderr, "name '%s' not found at offset %td\n",
		ip->id, ip - initialize_forth);
	abort();
    }
    return NAME_XT(name);
}


PRIM_HDLR(meta_interpret)
{
    while (ip->id != NULL) {
	execute(vm, meta_lookup(vm, ip));
	ip++;
    }
    return ip + 1;
}


PRIM_HDLR(meta_compile)
{
    while (ip->id != NULL) {
	COMPILE(vm, meta_lookup(vm, ip));
	ip++;
    }
    return ip + 1;
}


PRIM_HDLR(i_lookup)
{
    PUSH(vm, meta_lookup(vm, ip));
    return ip + 1;
}
