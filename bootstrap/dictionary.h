/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

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

extern name_p lookup(vmstate_p, c_addr_ft, cell_ft);

/*
 * C definitions and declarations relating to the Forth dictionary,
 * including structures for initialization.
 *
 * For ALIGN to produce an aligned data space pointer, the
 * dictionary space must be cell-aligned.  To prevent ALIGN from
 * overflowing the dictionary space, DICTIONARY_SIZE must also be a
 * multiple of CELL_SIZE.
 */

#define MAX_SEARCH_ORDER	8

#define DICTIONARY_SIZE		(0x10000 XCELLS)

extern union dict {
    struct {
	cell_ft		here;		    /* HERE */
	name_p		forth_wordlist;	    /* FORTH-WORDLIST */
	name_p *	current;	    /* CURRENT */
	cell_ft		n_search_order;
	name_p *	search_order[MAX_SEARCH_ORDER];

	size_t		lineno;
	FILE *		input;
    } dict_static_data;
    addr_unit_ft	dict_space[DICTIONARY_SIZE];
} dictionary;

#define DICT		dictionary.dict_static_data
#define HERE		(dictionary.dict_space + DICT.here)

#define STATE_INTERP		F_FALSE
#define STATE_COMPILE		F_TRUE

#define SOURCE_ID_TERMINAL	((cell_ft) 0)
#define SOURCE_ID_EVALUATE	((cell_ft) -1)


/*
 * Miscellaneous C utility declarations.
 */

extern addr_ft allot(vmstate_p, cell_ft);

#define COMMA(vm, x)	(*(a_addr_ft)allot((vm), CELL_SIZE) = (cell_ft) (x))
#define COMPILE(vm, xt)	COMMA(vm, xt)
#define XALIGN(vm)	(DICT.here = XALIGNED(DICT.here))

extern void interpret_lines(vmstate_p, char **);
extern int interpret_file(vmstate_p, char *);

extern vminstr_d initialize_forth[];
extern char *init_forth_defs[];

#endif // DICTIONARY_H
