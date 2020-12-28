/*
 * Copyright 2020, by J. Richard Barnette. All Rights Reserved.
 */

#include <stddef.h>

#include "forth.h"
#include "dictionary.h"

/*
 * allot.c - C internal functions for allocating dictionary data
 *    space during direct-mode meta-interpretation and -compilation.
 */


/* C equivalent to Forth ALLOT */
addr_ft
allot(vmstate_p vm, size_t n)
{
    addr_ft	h = DICT.here;
    addr_ft	nh = h + n;

    if (nh >= DICTIONARY_END)	THROW(vm, -8);
    DICT.here = nh;

    return h;
}
