/*
 * Copyright 2007, by J. Richard Barnette
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stddef.h>

#include "forth.h"
#include "vm.h"

#define NAME_MAX_LENGTH		31
#define NAME_IDENT_LENGTH(nm)	((nm)->len & 0x1f)
#define NAME_CFA_OFFSET(nm)	\
	ALIGNED(NAME_IDENT_LENGTH(nm) + offsetof(struct name_header, ident))

typedef struct name_header *	name_p;

struct name_header {
    name_p		prev;
    unsigned char	len;
    unsigned char	ident[NAME_MAX_LENGTH];
};

#define DICTIONARY_SIZE		0x10000

typedef struct {
    cell_ft		here;
    name_p		namelist;
    addr_unit_ft	dict[DICTIONARY_SIZE];
} dictionary_t;

extern addr_ft allot(cell_ft);
extern xt_ft lookup(char *);
extern name_p addname(char *, vminstr_fn);

#endif
