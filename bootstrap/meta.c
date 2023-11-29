/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "forth.h"
#include "dictionary.h"
#include "meta.h"

/*
 * meta.c - Forth primitives to enable meta-interpret and
 *   meta-compilation.
 */


PRIM_HDLR(meta_interpret)
{
    while (ip->cell != 0) {
	execute(vm, reloc_entries[ip->cell]);
	ip++;
    }
    return ip + 1;
}


PRIM_HDLR(meta_compile)
{
    while (ip->cell != 0) {
	COMPILE(vm, reloc_entries[ip->cell]);
	ip++;
    }
    return ip + 1;
}


PRIM_HDLR(i_reference)
{
    PUSH(vm, reloc_entries[ip->cell]);
    return ip + 1;
}
