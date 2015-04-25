/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "forth.h"

/*
 * dictionary.c - C internal functions and Forth standard words for
 *   allocating and managing dictionary data space.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// ,                     6.1.0150 CORE                   27
// ALIGN                 6.1.0705 CORE                   33
// ALLOT                 6.1.0710 CORE                   33
// C,                    6.1.0860 CORE                   34
// HERE                  6.1.1650 CORE                   39
//
// UNUSED                6.2.2395 CORE EXT               57
//------  ------  ------  ------  ------  ------  ------  ------


union dict dictionary;


/*
 * Allocate "n" character units of dictionary space.  Return a
 * pointer to the allocated space.
 */
addr_ft
allot(vmstate_p vm, size_t n)
{
    cell_ft	h = DICT.here;
    cell_ft	nh = h + n;

    if (nh >= DICTIONARY_SIZE)	THROW(vm, -8);
    DICT.here = nh;

    return dictionary.dict_space + h;
}


/* -------------------------------------------------------------- */

/* ( x -- ) */
static vminstr_p
x_comma(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    *(a_addr_ft)allot(vm, CELL_SIZE) = POP(vm);
    return ip;
}


/* ( -- ) */
static vminstr_p
x_align(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    XALIGN(vm);
    return ip;
}


/* ( n -- ) */
static vminstr_p
x_allot(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    (void) allot(vm, POP(vm));
    return ip;
}


/* ( char -- ) */
static vminstr_p
x_c_comma(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    *(c_addr_ft)allot(vm, sizeof (char_ft)) = (char_ft) POP(vm);
    return ip;
}


/* ( -- addr ) */
static vminstr_p
x_here(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, HERE);
    return ip;
}


/* ( -- u ) */
static vminstr_p
x_unused(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, DICTIONARY_SIZE - DICT.here);
    return ip;
}


DIRECT_FORTH(init_dictionary) // {
    L(sizeof (dictionary.dict_static_data))
	L(&DICT.here)			X(x_store)
    L(1)
	L(&DICT.n_search_order)		X(x_store)
    L(&DICT.forth_wordlist) X(x_dup)
	L(&DICT.current)		X(x_store)
	L(&DICT.search_order[0])	X(x_store)

    PRIM(",",         x_comma)
    PRIM("ALIGN",     x_align)
    PRIM("ALLOT",     x_allot)
    PRIM("C,",        x_c_comma)
    PRIM("HERE",      x_here)
    PRIM("UNUSED",    x_unused)
END_DIRECT // }
