/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef NAMES_H
#define NAMES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "forth.h"

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
	    ((xt_ft) ((addr_ft)(nm) + NAME_SIZE(NAME_LENGTH(nm))))

typedef struct name_header *	name_p;

struct name_header {
    name_p		prev;
    char_ft		flags;
    char_ft		ident[NAME_MAX_LENGTH];
};

extern name_p lookup(vmstate_ft *, c_addr_ft, cell_ft);

#endif // NAMES_H
