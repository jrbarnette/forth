/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stddef.h>
#include <stdio.h>

#include "forth.h"
#include "names.h"

/*
 * C definitions and declarations relating to the Forth dictionary,
 * including structures and routines for initialization.
 */

/*
 * For ALIGN to produce an aligned data space pointer, the
 * dictionary space must be cell-aligned.  To prevent ALIGN from
 * overflowing the dictionary space, DICTSPACE_SIZE must also be a
 * multiple of CELL_SIZE.
 */

#define DICTSPACE_SIZE		(0x10000 XCELLS)

extern union dict {
    struct {
	addr_ft		here;		    /* HERE */
	name_ft		forth_wordlist;	    /* FORTH-WORDLIST */

	size_t		lineno;
	FILE *		input;
    } dict_static_data;
    addr_unit_ft	dict_space[DICTSPACE_SIZE];
} dictionary;

#define DICT			(dictionary.dict_static_data)
#define DICTSPACE_START		(dictionary.dict_space)
#define DICTSPACE_END		(dictionary.dict_space + DICTSPACE_SIZE)
#define HERE			(DICT.here)
#define FORTH_WORDLIST		(DICT.forth_wordlist)
#define XALIGN(vm)		(HERE = (addr_ft) XALIGNED((cell_ft) HERE))

/*
 * C utility functions used by main() to invoke the inner VM (both
 * direct and indirect threaded).
 */

extern name_ft lookup(vmstate_ft *, c_addr_ft, cell_ft);

extern void report_exception(int, vmstate_ft *, char *);
extern void execute(vmstate_ft *, xt_ft);

extern vminstr_ft initialize_forth[];
extern char *init_forth_defs[];

#endif // DICTIONARY_H
