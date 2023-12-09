/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <stddef.h>
#include <string.h>
#include <assert.h>

#include "forth.h"
#include "dictionary.h"
#include "meta.h"

/*
 * C definitions and declarations relating to "name space" in the
 * dictionary.  See section 3.3.1, "Name space".
 */

#define NAME_TYPE_DEFAULT		0x00
#define NAME_TYPE_NO_INTERPRET		0x40
#define NAME_TYPE_IMMEDIATE		0x80
#define NAME_TYPE_COMPILE		0xc0
#define NAME_IS_IMMEDIATE(nm)		(((nm)->flags & 0x80) != 0)
#define NAME_IS_INTERPRETABLE(nm)	(((nm)->flags & 0x40) == 0)
#define NAME_CLEAR_TYPE(nm)		((nm)->flags &= 0x1f)
#define NAME_SET_TYPE(nm, type)		((nm)->flags |= (type))
#define NAME_MAKE_IMMEDIATE(nm)		NAME_SET_TYPE(NAME_TYPE_IMMEDIATE)
#define NAME_MAX_LENGTH			31
#define NAME_LENGTH(nm)			((nm)->flags & 0x1f)
#define NAME_SIZE(len)			\
	    XALIGNED(offsetof(struct name_header, ident) + (len))
#define NAME_XT(nm)			\
	    ((xt_ft) ((addr_ft) (nm) + NAME_SIZE(NAME_LENGTH(nm))))

struct name_header {
    name_ft	prev;
    char_ft	flags;
    char_ft	ident[NAME_MAX_LENGTH];
};

/*
 * Routines for adding named definitions into the dictionary.
 */
static name_ft
addname(vmstate_ft *vm, c_addr_ft id, cell_ft len, vmhdlr_fn handler)
{
    if (len == 0)		THROW(vm, -16);
    if (len > NAME_MAX_LENGTH)	THROW(vm, -19);

    XALIGN(vm);
    name_ft name = (name_ft) allot(vm, NAME_SIZE(len) + CELL_SIZE);
    name->prev = FORTH_WORDLIST;
    name->flags = len;
    (void) memcpy(name->ident, id, len);

    xt_ft xtok = NAME_XT(name);
    xtok->handler = handler;
    *reloc_ptr++ = xtok;
    assert(HERE == xtok->arg->data);

    return name;
}


static void
linkname(name_ft name)
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
    name_ft name = addname(vm, (c_addr_ft) id, len, handler);
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
    linkname((name_ft) POP(vm));
    return ip;
}
