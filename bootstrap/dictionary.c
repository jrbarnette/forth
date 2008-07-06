/*
 * Copyright 2008, by J. Richard Barnette
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "forth.h"

#define	HERE		dict_here
#define NAME_ALIGNMENT	sizeof (cell_ft)
#define ALIGN(n)	((void) (HERE = (HERE + (n)-1) & -(n)))
#define NAUNITS(n)	(((n) + sizeof (addr_unit_ft) - 1) / sizeof (addr_unit_ft))

union dict dictionary = {
    .dict_static_data = {
	sizeof (dictionary.dict_static_data)
    }
};

/*
 * Allocate "n" character units of dictionary space.  Return a
 * pointer to the allocated space.
 */
addr_ft
allot(size_t n)
{
    cell_ft	h = HERE;
    cell_ft	nh = HERE + NAUNITS(n);

    if (nh >= DICTIONARY_SIZE) {
	(void) fprintf(stderr, "out of dictionary space\n");
	abort();
    }
    HERE = nh;

    return dictionary.dict_space + h;
}

/*
 * Look up a definition in the dictionary, and return its execution
 * token.  Return NULL if not found.
 */
name_p
lookup(char *nm, size_t len)
{
    name_p	cur;

    for (cur = dict_namelist; cur != NULL; cur = cur->prev) {
	if (len != NAME_LENGTH(cur))
	    continue;
	if (memcmp(nm, cur->ident, len) != 0)
	    continue;

	return cur;
    }

    return NULL;
}

/*
 * Add a definition into the dictionary and store the given function
 * pointer in the code field.
 */
name_p
addname(char *n, vminstr_fn cf_fn)
{
    name_p	cur;
    size_t	len = strlen(n);
    xt_ft	xtok;

    assert(len > 0 && len <= NAME_MAX_LENGTH);

    cur = (name_p) ALIGNED(HERE);
    cur->prev = dict_namelist;
    cur->len = len;
    (void) memcpy(cur->ident, n, len + 1);
    xtok = NAME_CFA(cur);
    xtok->handler = cf_fn;
    HERE = (cell_ft) (xtok->data - dictionary.dict_space);

    return dict_namelist = cur;
}

/* -------------------------------------------------------------- */

/* HERE ( -- addr ) */
static cell_ft
x_here(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, tos);

    return (cell_ft)(dictionary.dict_space + HERE);
}

/* ALLOT ( n -- ) */
static cell_ft
x_allot(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    HERE += tos;

    if (HERE >= DICTIONARY_SIZE) {
	(void) fprintf(stderr, "out of dictionary space\n");
	abort();	/* XXX -8 THROW */
    }

    return POP(vm);
}

/* ALIGN ( -- ) */
static cell_ft
x_align(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    ALIGN(CELL_ALIGNMENT);
    return tos;
}

/* , ( x -- ) */
static cell_ft
x_comma(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    *(cell_ft *)(dictionary.dict_space + HERE) = tos;
    HERE += CELL_SIZE;

    if (HERE >= DICTIONARY_SIZE) {
	(void) fprintf(stderr, "out of dictionary space\n");
	abort();	/* XXX -8 THROW */
    }

    return POP(vm);
}

/* C, ( char -- ) */
static cell_ft
x_c_comma(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    *(char_ft *)(dictionary.dict_space + HERE) = (char_ft)tos;
    HERE += sizeof (char_ft);

    if (HERE >= DICTIONARY_SIZE) {
	(void) fprintf(stderr, "out of dictionary space\n");
	abort();	/* XXX -8 THROW */
    }

    return POP(vm);
}

/* FIND ( c-addr -- c-addr 0 | xt 1 | xt -1 ) */
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
	PUSH(vm, NAME_CFA(nm));
	if (nm->len & NAME_IMMEDIATE) {
	    return 1;
	} else {
	    return -1;
	}
    }
}

/* ' ( "<spaces>name" -- xt ) */
static cell_ft
x_tick(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, tos);

    return 0;
}

/* IMMEDIATE ( -- ) */
static cell_ft
x_immediate(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    dict_namelist->len |= NAME_IMMEDIATE;
    return tos;
}

defn_dt
dictionary_defns[] = {
    { "HERE",      x_here },
    { "ALLOT",     x_allot },
    { "ALIGN",     x_align },
    { ",",         x_comma },
    { "C,",        x_c_comma },

/*  { "'",         x_tick }, */
    { "FIND",      x_find },

    { "IMMEDIATE", x_immediate },
    { NULL }
};
