/*
 * Copyright 2009, by J. Richard Barnette
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

/* , "comma"		6.1.0150 CORE, p. 27 */
/* ( x -- ) */
static cell_ft
x_comma(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    *(a_addr_ft)allot(vm, CELL_SIZE) = tos;
    return POP(vm);
}


/* ALIGN		6.1.0705 CORE, p. 33 */
/* ( -- ) */
static cell_ft
x_align(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    ALIGN(vm);
    return tos;
}


/* ALLOT		6.1.0710 CORE, p. 33 */
/* ( n -- ) */
static cell_ft
x_allot(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    (void) allot(vm, tos);
    return POP(vm);
}


/* C, "c-comma"		6.1.0860 CORE, p. 34 */
/* ( char -- ) */
static cell_ft
x_c_comma(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    *(c_addr_ft)allot(vm, sizeof (char_ft)) = (char_ft) tos;
    return POP(vm);
}


/* HERE			6.1.1650 CORE, p. 40 */
/* ( -- addr ) */
static cell_ft
x_here(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, tos);
    return (cell_ft) HERE;
}


static cell_ft
initialize_dictionary(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    DICT.here = sizeof (dictionary.dict_static_data);
    DICT.base = 10;
    return tos;
}


defn_dt
dictionary_defns[] = {
    { initialize_dictionary },
    { define_name, ",",         x_comma },
    { define_name, "ALIGN",     x_align },
    { define_name, "ALLOT",     x_allot },
    { define_name, "C,",        x_c_comma },
    { define_name, "HERE",      x_here },
    { NULL }
};

#if 0
    PAD                   6.2.2000 CORE EXT               56
    UNUSED                6.2.2395 CORE EXT               59
#endif
