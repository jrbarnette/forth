/*
 * Copyright 2020, by J. Richard Barnette. All Rights Reserved.
 */

#include <stddef.h>

#include "forth.h"
#include "dictionary.h"
#include "direct.h"

/*
 * allot.c - C internal functions for allocating dictionary data
 *    space during direct-mode meta-interpretation and -compilation.
 */


/* C equivalent to Forth ALLOT */
addr_ft
allot(vmstate_ft *vm, size_t n)
{
    addr_ft	h = HERE;
    addr_ft	nh = h + n;

    if (nh >= DICTSPACE_END)	THROW(vm, -8);
    HERE = nh;

    return h;
}
