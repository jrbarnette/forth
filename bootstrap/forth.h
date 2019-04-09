/*
 * Copyright 2015, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef FORTH_H
#define FORTH_H

#include <setjmp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


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

#define SHIFT(s)	((((s) & 0xaa) != 0) | ((((s) & 0xcc) != 0) << 1) | \
			    ((((s) & 0xf0) != 0) << 2))

#define CELL_SIZE	(sizeof (cell_ft))
#define XALIGNED(n)	(((n) + CELL_SIZE-1) & -CELL_SIZE)
#define XCELLS		* CELL_SIZE
#define CELL_SHIFT	SHIFT(CELL_SIZE)

/* The following would change if char_ft changes (e.g. to UTF-16) */
#define CHAR_SIZE	(sizeof (char_ft))
#define XCHARS
#define CHAR_SHIFT	0


/*
 * Virtual Machine State
 *
 * The virtual machine interpreter uses a more-or-less traditional
 * indirect threaded approach:  A Forth definition consists of a
 * pointer to a handler function that executes the defintion,
 * followed by definition data.  The definition is invoked by
 * calling the handler with the VM state and the address of the
 * definition data.
 *
 * The "execution token" for a definition is simply its address.
 * The execution token also serves as the representation of a
 * virtual machine instruction.
 *
 * The defintion data for a colon definition is an array of
 * virtual machine instructions (execution tokens) to be executed in
 * order.  The execution logic and the data structures come together
 * in these key functions: execute(), do_colon(), do_exit().
 */

#define STACK_SIZE	256
#define RSTACK_SIZE	512

typedef struct vmstate *		vmstate_p;
typedef union definition_data		definition_d;
typedef union definition_data *		xt_ft;
typedef union instruction_data		vminstr_d;
typedef union instruction_data *	vminstr_p;
typedef union argument_data		vmarg_d;
typedef union argument_data *		vmarg_p;

typedef vminstr_p (*vminstr_fn)(vminstr_p, vmstate_p, vmarg_p);

struct vmstate {
    a_addr_ft	sp;
    a_addr_ft	rsp;
    cell_ft	stack[STACK_SIZE];
    cell_ft	rstack[RSTACK_SIZE];
    jmp_buf	interp_loop;
};

union instruction_data {
    /* general purpose VM instructions */
    xt_ft		xtok;
    cell_ft		cell;
    snumber_ft		offset;
    char_ft		cdata[1];

    /* direct execution only */
    vminstr_fn		handler;
    vminstr_p		ip;
    char *		id;
    void *		ptr;
};

union argument_data {
    vminstr_p		ip;
    xt_ft		xtok;
    cell_ft		cell;
    char_ft		cdata[1];
    addr_unit_ft	data[1];
    vminstr_d		vminstrs[1];
};

union definition_data {
    vminstr_fn		handler;
    vmarg_d		arg[1];
};

#define CLEAR_STACK(vm)		((vm)->sp = (vm)->stack + STACK_SIZE)
#define CLEAR_RSTACK(vm)	((vm)->rsp = (vm)->rstack + RSTACK_SIZE)
#define DEPTH(vm)		(STACK_SIZE - ((vm)->sp - (vm)->stack))
#define EMPTY(vm)		((vm)->sp == (vm)->stack + STACK_SIZE)
#define REMPTY(vm)		((vm)->rsp == (vm)->rstack + RSTACK_SIZE)
#define SP(vm)			((vm)->sp)
#define RSP(vm)			((vm)->rsp)
#define PICK(sp, n)		((sp)[(n)])
#define SET_SP(vm, nsp, n)	((vm)->sp = (nsp) + (n))
#define SET_RSP(vm, nsp, n)	((vm)->rsp = (nsp) + (n))

#define THROW(vm, n)		(longjmp((vm)->interp_loop, (n)))

#ifdef CHECKMODE
#if CHECKMODE == 1
#define CHECK(vm, t, x)	{ if (!(t)) return (THROW(vm, (x)), 0); }
#elif CHECKMODE == 2
#define CHECK(vm, t, x)	((t) || (THROW(vm, (x)), 0))
#endif
#endif

#ifndef CHECK
#define CHECK(vm, t, x)
#endif

#define CHECK_PUSH(vm, n)	\
	    CHECK(vm, (vm)->sp >= (vm)->stack + (n), -3)
#define CHECK_POP(vm, n)	\
	    CHECK(vm, (vm)->sp <= (vm)->stack + STACK_SIZE - (n), -4)
#define CHECK_RPUSH(vm, n)	\
	    CHECK(vm, (vm)->rsp >= (vm)->rstack + (n), -5)
#define CHECK_RPOP(vm, n)	\
	    CHECK(vm, (vm)->rsp <= (vm)->rstack + RSTACK_SIZE - (n), -6)

#define POP(vm)		(*(vm)->sp++)
#define PUSH(vm, c)	(*--(vm)->sp = (cell_ft)(c))
#define RPOP(vm)	(*(vm)->rsp++)
#define RPUSH(vm, c)	(*--(vm)->rsp = (cell_ft)(c))


/*
 * C definitions and declarations relating to "name space" in the
 * dictionary.  See section 3.3.1, p 15.
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

#define MAX_SEARCH_ORDER	8


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
	cell_ft		here;		    /* HERE */
	name_p		forth_wordlist;	    /* FORTH-WORDLIST */
	name_p *	current;	    /* CURRENT */
	cell_ft		n_search_order;
	name_p *	search_order[MAX_SEARCH_ORDER];

	cell_ft		state;		    /* STATE */

	/* the input source and parse area - 4 cells total */
	cell_ft		to_in;		    /* >IN */
	cell_ft		source_id;	    /* SOURCE-ID */
	string_ft	source;		    /* SOURCE */

	char_ft		tib[256];	    /* TIB */
	cell_ft		source_max_len;     /* #TIB */
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

#define PARSE_AREA_PTR		(DICT.source.c_addr + DICT.to_in)
#define PARSE_AREA_LEN		(DICT.source.len - DICT.to_in)


/*
 */

extern addr_ft allot(vmstate_p, cell_ft);

extern void execute(vmstate_p vm, xt_ft entry_xt);

extern cell_ft parse(char_ft, c_addr_ft, cell_ft);
extern c_addr_ft parse_name(cell_ft *);
extern cell_ft refill(void);

extern void quit(vmstate_p, FILE *);
extern void interpret_lines(vmstate_p, char **);

#define COMMA(vm, x)	(*(a_addr_ft)allot((vm), CELL_SIZE) = (cell_ft) (x))
#define COMPILE(vm, xt)	COMMA(vm, xt)
#define XALIGN(vm)	(DICT.here = XALIGNED(DICT.here))

extern vminstr_d initialize_forth[];

extern char *init_forth_defs[];

#endif
