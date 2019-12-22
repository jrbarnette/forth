/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "forth.h"
#include "dictionary.h"

/*
 * names.c - C internal functions and Forth standard words for
 *   dictionary name space.
 */

/*------  ------  ------  ------  ------  ------  ------  ------
 * '                     6.1.0070 CORE                   25
 * FIND                  6.1.1550 CORE                   39
 *
 * SEARCH-WORDLIST    16.6.1.2192 SEARCH                120
 *------  ------  ------  ------  ------  ------  ------  ------
 */


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
    assert(HERE == xtok->arg->cdata);

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
i_linkname(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    linkname((name_p) POP(vm));
    return ip;
}
