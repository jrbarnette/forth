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

#define DICTIONARY_SIZE		(0x10000 XCELLS)

extern union dict {
    struct {
	addr_ft		here;		    /* HERE */
	name_p		forth_wordlist;	    /* FORTH-WORDLIST */

	size_t		lineno;
	FILE *		input;
    } dict_static_data;
    addr_unit_ft	dict_space[DICTIONARY_SIZE];
} dictionary;

#define DICT		(dictionary.dict_static_data)
#define DICTIONARY_END	(&dictionary.dict_space[DICTIONARY_SIZE])
#define HERE		(DICT.here)
#define FORTH_WORDLIST	(DICT.forth_wordlist)


/*
 * Miscellaneous C utility declarations.
 */

extern addr_ft allot(vmstate_ft *, cell_ft);

#define COMMA(vm, x)	(*(a_addr_ft)allot((vm), CELL_SIZE) = (cell_ft) (x))
#define COMPILE(vm, xt)	COMMA(vm, xt)
#define XALIGN(vm)	(DICT.here = (addr_ft)XALIGNED((cell_ft) DICT.here))

extern vminstr_ft initialize_forth[];
extern char *init_forth_defs[];

#endif // DICTIONARY_H
