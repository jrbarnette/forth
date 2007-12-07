/*
 * Copyright 2007, by J. Richard Barnette
 */

#ifndef FORTH_H
#define FORTH_H

#include <stddef.h>

typedef unsigned char		addr_unit_ft;
typedef addr_unit_ft *		addr_ft;

typedef uintptr_t		cell_ft;
typedef cell_ft *		a_addr_ft;

typedef unsigned char		char_ft;
typedef char_ft *		c_addr_ft;

typedef struct {
    cell_ft		hi;
    cell_ft		lo;
} dcell_ft;

typedef struct {
    cell_ft		len;
    cell_ft		c_addr;
} string_ft;

#define CELL_ALIGNMENT	(sizeof (cell_ft))
#define ALIGNED(n)	(((n) + CELL_ALIGNMENT - 1) & -CELL_ALIGNMENT)

#endif
