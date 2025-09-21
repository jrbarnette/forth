/*
 * Copyright 2020, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef FTYPES_H
#define FTYPES_H

#include <stdint.h>

/*
 * Basic Forth types and constants directly visible to Forth code.
 * The types are formally defined in the ANS Forth spec, section
 * 3.1, p. 8.
 *
 * addr_unit:	Smallest addressable unit of storage
 * char:	Character value (superset of ASCII)
 * cell:	( x ) Standard stack entry
 * snumber:	( n ) Single cell signed number
 * unumber:	( u ) Single cell unsigned number
 * flag:	( true | false ) boolean flag value
 * addr:	Cell-sized value of an address
 * a-addr:	Cell-aligned address
 * c-addr:	Character-aligned address
 * dcell:	( du ) Two cells, laid out according to 3.1.4
 * string:	( c-addr u ) Pair designating a string
 */

typedef uint8_t			addr_unit_ft;
typedef addr_unit_ft *		addr_ft;

typedef unsigned char		char_ft;
typedef char_ft *		c_addr_ft;

typedef intptr_t		snumber_ft;
typedef uintptr_t		unumber_ft;

typedef uintptr_t		cell_ft;
typedef cell_ft *		a_addr_ft;

typedef cell_ft			flag_ft;
#define F_FALSE			((flag_ft) 0)
#define F_TRUE			((flag_ft) ~0)

typedef struct {
    cell_ft		hi;
    cell_ft		lo;
} dcell_ft;

typedef struct {
    cell_ft		len;
    c_addr_ft		c_addr;
} string_ft;


#define CELL_SIZE	(sizeof (cell_ft))
#define XALIGNED(n)	(((n) + CELL_SIZE-1) & -CELL_SIZE)
#define XCELLS		* CELL_SIZE

#define CHAR_SIZE	(sizeof (char_ft))
#define XCHARS		* CHAR_SIZE

#endif // FTYPES_H
