/*
 * Copyright 2009, by J. Richard Barnette
 */

#ifndef FORTH_H
#define FORTH_H

#include <stdint.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stddef.h>


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
#define CELL_ALIGNMENT	(sizeof (cell_ft))
#define ALIGNED(n)	(((n) + CELL_ALIGNMENT - 1) & -CELL_ALIGNMENT)
#define CELLS		* CELL_SIZE


/*
 * VM specific definitions not directly visible to Forth code.
 */

#define STACK_SIZE	2048
#define RSTACK_SIZE	64

typedef union vminstr *		xt_ft;
typedef struct vmstate *	vmstate_p;

struct vmstate {
    jmp_buf	interp_loop;
    xt_ft *	ip;
    size_t	sp;
    size_t	rsp;
    cell_ft	stack[STACK_SIZE];
    cell_ft	rstack[RSTACK_SIZE];
};

typedef cell_ft (*vminstr_fn)(cell_ft, vmstate_p, addr_ft);

union vminstr {
    vminstr_fn		handler;
    addr_unit_ft	data[1];
    snumber_ft		snum;
    cell_ft		cell;
};

#define CLEAR_STACK(vm)		((vm)->sp = STACK_SIZE)
#define CLEAR_RSTACK(vm)	((vm)->rsp = RSTACK_SIZE)
#define DEPTH(vm)		(STACK_SIZE - (vm)->sp)
#define EMPTY(vm)		((vm)->sp == STACK_SIZE)
#define PICK(vm, n)		((vm)->stack[(vm)->sp + (n)])
#define SP(vm)			(&(vm)->stack[(vm)->sp])

#define THROW(vm, n)		(longjmp((vm)->interp_loop, (n)))
#define CHECK_PUSH(vm, n)	\
	    (((vm)->sp >= (n)) || (THROW(vm, -3), 0))
#define CHECK_POP(vm, n)	\
	    (((vm)->sp + (n) <= STACK_SIZE) || (THROW(vm, -4), 0))
#define CHECK_RPUSH(vm, n)	\
	    (((vm)->rsp >= (n)) || (THROW(vm, -5), 0))
#define CHECK_RPOP(vm, n)	\
	    (((vm)->rsp + (n) <= RSTACK_SIZE) || (THROW(vm, -6), 0))

#define POP(vm)		((vm)->stack[(vm)->sp++])
#define PUSH(vm, c)	((vm)->stack[--(vm)->sp] = (cell_ft)(c))
#define RPOP(vm)	((vm)->rstack[(vm)->rsp++])
#define RPUSH(vm, c)	((vm)->rstack[--(vm)->rsp] = (cell_ft)(c))

#define SAVEDTOS(vm)	((vm)->stack[STACK_SIZE-1])


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
	    ALIGNED(offsetof(struct name_header, ident) + (len))
#define NAME_XT(nm)			\
	    ((xt_ft) ((addr_ft)(nm) + NAME_SIZE(NAME_LENGTH(nm))))

typedef struct name_header *	name_p;

struct name_header {
    name_p		prev;
    unsigned char	flags;
    unsigned char	ident[NAME_MAX_LENGTH];
};

extern name_p lookup(c_addr_ft, size_t);
extern xt_ft find(c_addr_ft, size_t);


/*
 * C definitions and declarations relating to the Forth dictionary,
 * including structures for initialization.
 *
 * For ALIGN() to produce an aligned data space pointer, the
 * dictionary space must be cell-aligned.  To prevent ALIGN() from
 * overflowing the dictionary space, DICTIONARY_SIZE must also be a
 * multiple of CELL_SIZE.
 */

#define DICTIONARY_SIZE		(0x10000 CELLS)

extern union dict {
    struct {
	cell_ft		here;		/* HERE */
	name_p		namelist;	/* internal */

	union vminstr	literal_instr;	/* for LITERAL runtime xt */
	union vminstr	skip_instr;	/* for ELSE runtime xt */
	union vminstr	fskip_instr;	/* for IF runtime xt */
	union vminstr	tskip_instr;	/* for IF runtime xt */

	cell_ft		state;		/* STATE */

	char_ft		tib[256];	/* TIB */

	/* the input source and parse area - 4 cells total */
	cell_ft		source_id;	/* SOURCE-ID */
	string_ft	source;		/* SOURCE */
	cell_ft		to_in;		/* >IN */
	int		source_max_len;

	cell_ft		base;		/* BASE */
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

#define DO_LITERAL_XT		(&DICT.literal_instr)
#define SKIP_XT			(&DICT.skip_instr)
#define FSKIP_XT		(&DICT.fskip_instr)
#define TSKIP_XT		(&DICT.tskip_instr)

extern addr_ft allot(vmstate_p, cell_ft);

typedef void (*definer_fn)(name_p, void *);

typedef struct defn {
    vminstr_fn		fn;
    void *		data[3];
} defn_dt;

extern defn_dt primitive_defns[];
extern defn_dt dictionary_defns[];
extern defn_dt format_defns[];
extern defn_dt compile_defns[];

extern cell_ft define_name(cell_ft, vmstate_p, addr_ft);

extern cell_ft do_colon(cell_ft, vmstate_p, addr_ft);
extern cell_ft do_variable(cell_ft, vmstate_p, addr_ft);
extern cell_ft do_constant(cell_ft, vmstate_p, addr_ft);

extern cell_ft do_else(cell_ft, vmstate_p, addr_ft);
extern cell_ft do_if(cell_ft, vmstate_p, addr_ft);
extern cell_ft do_literal(cell_ft, vmstate_p, addr_ft);


/*
 */

extern void execute(vmstate_p, xt_ft);
extern cell_ft parse(char_ft, c_addr_ft, cell_ft);

#define COMMA(vm, x)	(*(a_addr_ft)allot((vm), CELL_SIZE) = (cell_ft) (x))
#define ALIGN(vm)	(DICT.here = ALIGNED(DICT.here))

#endif
