/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "forth.h"
#include "direct.h"
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
 * Look up a definition in forth_wordlist, and return a pointer to
 * its name header.  Return NULL if not found.
 */
name_p
lookup(vmstate_ft *vm, c_addr_ft id, cell_ft len)
{
    if (len == 0)
	THROW(vm, -16);

    for (name_p cur = FORTH_WORDLIST; cur != NULL; cur = cur->prev) {
	if (len != NAME_LENGTH(cur))
	    continue;

	cell_ft i = 0;
	while (toupper(id[i]) == toupper(cur->ident[i])) {
	    i++;
	    if (i == len)
		return cur;
	}
    }
    return NULL;
}


/*
 * Routines for adding named definitions into the dictionary.
 */
static name_p
addname(vmstate_ft *vm, c_addr_ft id, cell_ft len, vmhdlr_fn handler)
{
    if (len == 0)		THROW(vm, -16);
    if (len > NAME_MAX_LENGTH)	THROW(vm, -19);

    XALIGN(vm);
    name_p name = (name_p) allot(vm, NAME_SIZE(len) + CELL_SIZE);
    name->prev = FORTH_WORDLIST;
    name->flags = len;
    (void) memcpy(name->ident, id, len);

    xt_ft xtok = NAME_XT(name);
    xtok->handler = handler;
    assert(HERE == xtok->arg->data);

    return name;
}


static void
linkname(name_p name)
{
    FORTH_WORDLIST = name;
}


/* -------------------------------------------------------------- */

PRIM_HDLR(i_startname)
{
    CHECK_PUSH(vm, 1);
    char *id = (ip++)->id;
    cell_ft len = strlen(id);
    vmhdlr_fn handler = (ip++)->handler;
    name_p name = addname(vm, (c_addr_ft) id, len, handler);
    PUSH(vm, name);

    return ip;
}


PRIM_HDLR(i_addname)
{
    char *id = (ip++)->id;
    cell_ft len = strlen(id);
    linkname(addname(vm, (c_addr_ft) id, len, (ip++)->handler));

    return ip;
}


PRIM_HDLR(i_setflags)
{
    NAME_SET_TYPE(FORTH_WORDLIST, ip->cell);
    return ip + 1;
}


PRIM_HDLR(i_linkname)
{
    CHECK_POP(vm, 1);
    linkname((name_p) POP(vm));
    return ip;
}
