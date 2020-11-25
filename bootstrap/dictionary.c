/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "forth.h"
#include "dictionary.h"

/*
 * dictionary.c - C internal functions and Forth standard words for
 *   allocating and managing dictionary data space.
 */

/*------  ------  ------  ------  ------  ------  ------  ------
 * ,                     6.1.0150 CORE                   27
 * ALIGN                 6.1.0705 CORE                   33
 * ALLOT                 6.1.0710 CORE                   33
 * C,                    6.1.0860 CORE                   34
 * HERE                  6.1.1650 CORE                   39
 *
 * UNUSED                6.2.2395 CORE EXT               57
 *------  ------  ------  ------  ------  ------  ------  ------
 */


union dict dictionary;


/*
 * Allocate "n" character units of dictionary space.  Return a
 * pointer to the allocated space.
 */
addr_ft
allot(vmstate_p vm, size_t n)
{
    addr_ft	h = DICT.here;
    addr_ft	nh = h + n;

    if (nh >= DICTIONARY_END)	THROW(vm, -8);
    DICT.here = nh;

    return h;
}
