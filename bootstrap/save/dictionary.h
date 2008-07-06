/*
 * Copyright 2007, by J. Richard Barnette
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stddef.h>

#include "forth.h"
#include "vm.h"

#define NAME_IMMEDIATE		0x80
#define NAME_MAX_LENGTH		31
#define NAME_LENGTH(nm)		((nm)->len & 0x1f)
#define NAME_IDENT_ALIGN	\
	    (CELL_ALIGNMENT + offsetof(struct name_header, ident))
#define NAME_CFA_OFFSET(nm)	\
	    (((nm)->len + NAME_IDENT_ALIGN) & (-CELL_ALIGNMENT & 0x1f))
#define NAME_CFA(nm)		((xt_ft)((char *)(nm) + NAME_CFA_OFFSET(nm)))

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

typedef void (*definer_fn)(void *);

typedef struct defn {
    char *		name;
    vminstr_fn		fn;
    definer_fn		definer;
    void *		datap;
} defn_dt;

extern defn_dt primitives[];

#endif
