/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "forth.h"
#include "direct.h"

/*
 * names.c - C internal functions and Forth standard words for
 *   dictionary name space.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// '                     6.1.0070 CORE                   25
// FIND                  6.1.1550 CORE                   39
//
// SEARCH-WORDLIST    16.6.1.2192 SEARCH                120
//------  ------  ------  ------  ------  ------  ------  ------


/*
 * Look up a definition in a wordlist, and return a pointer to its
 * name header.  Return NULL if not found.
 */
static name_p
search_wordlist(name_p *names, c_addr_ft id, cell_ft len)
{
    name_p	cur;

    for (cur = *names; cur != NULL; cur = cur->prev) {
	int i;

	if (len != NAME_LENGTH(cur))
	    continue;

	i = 0;
	while (toupper(id[i]) == toupper(cur->ident[i])) {
	    i++;
	    if (i == len)
		return cur;
	}
    }

    return NULL;
}


/*
 * Look up a definition in the search order, and return a pointer to
 * its name header.  Return NULL if not found.
 */
name_p
lookup(vmstate_p vm, c_addr_ft id, cell_ft len)
{
    name_p nm = NULL;
    cell_ft i;

    if (len == 0)
	THROW(vm, -16);

    for (i = 0; i < DICT.n_search_order; i++) {
	nm = search_wordlist(DICT.search_order[i], id, len);
	if (nm != NULL) {
	    break;
	}
    }
    return nm;
}


/*
 * Routines for adding named definitions into the dictionary.
 */
static name_p
addname(vmstate_p vm, c_addr_ft id, cell_ft len, vminstr_fn handler)
{
    if (len == 0)		THROW(vm, -16);
    if (len > NAME_MAX_LENGTH)	THROW(vm, -19);

    XALIGN(vm);
    name_p name = (name_p) allot(vm, NAME_SIZE(len) + CELL_SIZE);
    name->prev = *DICT.current;
    name->flags = len;
    (void) memcpy(name->ident, id, len);

    xt_ft xtok = NAME_XT(name);
    xtok->handler = handler;
    assert(HERE == xtok[1].arg->data);

    return name;
}


static void
linkname(name_p name)
{
    *DICT.current = name;
}


/* -------------------------------------------------------------- */

vminstr_p
i_startname(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    char *id = (ip++)->id;
    cell_ft len = strlen(id);
    vminstr_fn handler = (ip++)->handler;
    name_p name = addname(vm, (c_addr_ft) id, len, handler);
    PUSH(vm, name);

    return ip;
}


vminstr_p
i_addname(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    char *id = (ip++)->id;
    cell_ft len = strlen(id);
    linkname(addname(vm, (c_addr_ft) id, len, (ip++)->handler));

    return ip;
}


vminstr_p
i_setflags(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    NAME_SET_TYPE(*DICT.current, ip->cell);
    return ip + 1;
}


vminstr_p
x_linkname(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    linkname((name_p) POP(vm));
    return ip;
}


vminstr_p
x_startname(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft len;
    c_addr_ft id = parse_name(&len);
    CHECK_POP(vm, 1);
    cell_ft *sp = SP(vm);
    vminstr_fn handler = (vminstr_fn) PICK(sp, 0);
    PICK(sp, 0) = (cell_ft) addname(vm, id, len, handler);

    return ip;
}


/* -------------------------------------------------------------- */

/* ( "<spaces>name" -- xt ) */
vminstr_p
x_tick(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft len;
    c_addr_ft id = parse_name(&len);
    name_p nm = lookup(vm, id, len);

    if (nm == NULL)
	THROW(vm, -13);

    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) NAME_XT(nm));
    return ip;
}


/* ( c-addr - c-addr 0 | xt -1 | xt 1 ) */
vminstr_p
x_find(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    CHECK_PUSH(vm, 1);
    a_addr_ft sp = SP(vm);
    c_addr_ft counted = (c_addr_ft) PICK(sp, 0);
    SET_SP(vm, sp, -1);

    name_p nm = lookup(vm, counted + 1, *counted);
    if (nm == NULL) {
	PICK(sp, -1) = 0;
    } else {
	PICK(sp, 0) = (cell_ft) NAME_XT(nm);
	PICK(sp, -1) = NAME_IS_IMMEDIATE(nm) ? 1 : -1;
    }
    return ip;
}


/* ( c-addr u wid -- 0 | xt 1 | xt -1 ) */
vminstr_p
x_search_wordlist(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    c_addr_ft caddr;
    cell_ft   len;
    name_p *  wl;

    CHECK_POP(vm, 3);
    wl = (name_p *) POP(vm);
    len = (cell_ft) POP(vm);
    caddr = (c_addr_ft) POP(vm);

    if (len == 0)
	THROW(vm, -16);

    name_p nm = search_wordlist(wl, caddr, len);
    if (nm == NULL) {
	PUSH(vm, 0);
    } else {
	PUSH(vm, NAME_XT(nm));
	PUSH(vm, NAME_IS_IMMEDIATE(nm) ? 1 : -1);
    }
    return ip;
}
