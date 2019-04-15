/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "forth.h"
#include "names.h"

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
