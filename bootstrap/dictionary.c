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

/*------  ------  ------  ------  ------  ------  ------  ------
  ,                     6.1.0150 CORE                   27
  ALIGN                 6.1.0705 CORE                   33
  ALLOT                 6.1.0710 CORE                   33
  C,                    6.1.0860 CORE                   34
  HERE                  6.1.1650 CORE                   39
  ------  ------  ------  ------  ------  ------  ------  ------
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


/*------  ------  ------  ------  ------  ------  ------  ------
  UNUSED                6.2.2395 CORE EXT               57
  ------  ------  ------  ------  ------  ------  ------  ------
*/

/* ( -- u ) */
static vminstr_p
x_unused(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, DICTIONARY_SIZE - DICT.here);
    return ip;
}


void
initialize_dictionary(vmstate_p vm, defn_data_p ignore)
{
    DICT.here = sizeof (dictionary.dict_static_data);
    DICT.current = &DICT.forth_wordlist;
    DICT.n_search_order = 1;
    DICT.search_order[0] = &DICT.forth_wordlist;
}


defn_dt
dictionary_defns[] = {
    { initialize_dictionary },
    { define_name, ",",         x_comma },
    { define_name, "ALIGN",     x_align },
    { define_name, "ALLOT",     x_allot },
    { define_name, "C,",        x_c_comma },
    { define_name, "HERE",      x_here },
    { define_name, "UNUSED",    x_unused },
    { NULL }
};
