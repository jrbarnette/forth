/*
 * Copyright 2009, by J. Richard Barnette
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "forth.h"

/*
 * dictionary.c - Implementation of C internal and Forth standard
 *   words for allocating and managing dictionary space.  Includes
 *   "data space" and "name space", as defined in the Forth
 *   standard.
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


/*
 * Look up a definition in the dictionary, and return its execution
 * token.  Return NULL if not found.
 */
name_p
lookup(c_addr_ft id, size_t len)
{
    name_p	cur;

    for (cur = DICT.namelist; cur != NULL; cur = cur->prev) {
	if (len != NAME_LENGTH(cur))
	    continue;
	if (memcmp(id, cur->ident, len) != 0)
	    continue;

	return cur;
    }

    return NULL;
}


xt_ft
find(c_addr_ft id, size_t len)
{
    name_p nm = lookup(id, len);
    return (nm == NULL) ? NULL : NAME_XT(nm);
}


/*
 * Routines for adding named definitions into the dictionary.
 */
static name_p
addname(vmstate_p vm, void **data)
{
    char *	id = (char *) data[0];
    vminstr_fn	hdlr = (vminstr_fn) data[1];
    cell_ft	type = (cell_ft) data[2];
    size_t	len = strlen(id);
    name_p	cur;
    xt_ft	xtok;

    if (len == 0)		THROW(vm, -16);
    if (len > NAME_MAX_LENGTH)	THROW(vm, -19);

    ALIGN(vm);
    cur = (name_p) allot(vm, NAME_SIZE(len) + CELL_SIZE);
    cur->prev = NULL;
    cur->flags = len;
    NAME_SET_TYPE(cur, type);
    (void) memcpy(cur->ident, id, len);
    xtok = NAME_XT(cur);
    xtok->handler = hdlr;
    assert(HERE == xtok[1].data);

    return cur;
}


cell_ft
define_name(cell_ft tos, vmstate_p vm, addr_ft data)
{
    name_p nm = addname(vm, (void **) data);
    nm->prev = DICT.namelist;
    DICT.namelist = nm;
    return tos;
}


/* -------------------------------------------------------------- */

/* ' "tick"		6.1.0070 CORE, p. 25 */
/* ( "<spaces>name" -- xt ) */
static cell_ft
x_tick(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, tos);

    /* XXX ' - parse name */

    return 0;
}


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


/* FIND			6.1.1550 CORE, p. 39 */
/* ( c-addr -- c-addr 0 | xt 1 | xt -1 ) */
static cell_ft
x_find(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    c_addr_ft caddr = (c_addr_ft) tos;
    name_p    nm;

    CHECK_POP(vm, 1);
    CHECK_PUSH(vm, 1);

    nm = lookup(caddr + 1, (size_t) *caddr);
    if (nm == NULL) {
	PUSH(vm, tos);
	return 0;
    } else {
	PUSH(vm, NAME_XT(nm));
	return NAME_IS_IMMEDIATE(nm) ? 1 : -1;
    }
}


/* HERE			6.1.1650 CORE, p. 40 */
/* ( -- addr ) */
static cell_ft
x_here(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, tos);
    return (cell_ft)HERE;
}


/* IMMEDIATE		6.1.1710 CORE, p. 41 */
/* ( -- ) */
static cell_ft
x_immediate(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    NAME_SET_TYPE(DICT.namelist, NAME_TYPE_IMMEDIATE);
    return tos;
}


static cell_ft
initialize_dictionary(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    DICT.here = sizeof (dictionary.dict_static_data);
    DICT.literal_instr.handler = do_literal;
    DICT.skip_instr.handler = do_else;
    DICT.fskip_instr.handler = do_if;
    DICT.base = 10;
    return tos;
}


defn_dt
dictionary_defns[] = {
    { initialize_dictionary },
/*  { define_name, "'",         x_tick }, */
    { define_name, ",",         x_comma },
    { define_name, "ALIGN",     x_align },
    { define_name, "ALLOT",     x_allot },
    { define_name, "C,",        x_c_comma },
    { define_name, "FIND",      x_find },
    { define_name, "HERE",      x_here },
    { define_name, "IMMEDIATE", x_immediate },
    { NULL }
};

#if 0
COMPILE,              6.2.0945 CORE EXT               54
MARKER                6.2.1850 CORE EXT               56
PAD                   6.2.2000 CORE EXT               56
UNUSED                6.2.2395 CORE EXT               59
#endif
