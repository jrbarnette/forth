/*
 * Copyright 2011, by J. Richard Barnette
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

#define CELL_SIZE	(sizeof (cell_ft))
#define CELL_ALIGNMENT	(sizeof (cell_ft))
#define ALIGNED(n)	(((n) + CELL_ALIGNMENT - 1) & -CELL_ALIGNMENT)
#define CELLS		* CELL_SIZE
#define CHARS		* (sizeof (char_ft))


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

#define STACK_SIZE	32
#define RSTACK_SIZE	64

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

    /* initialization instructions only */
    vminstr_fn		handler;
    char *		id;
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

extern void execute(vmstate_p, xt_ft);

#define CLEAR_STACK(vm)		((vm)->sp = vm->stack + STACK_SIZE)
#define CLEAR_RSTACK(vm)	((vm)->rsp = vm->rstack + RSTACK_SIZE)
#define DEPTH(vm)		(STACK_SIZE - ((vm)->sp - (vm)->stack))
#define EMPTY(vm)		((vm)->sp == (vm)->stack)
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
	    ALIGNED(offsetof(struct name_header, ident) + (len))
#define NAME_XT(nm)			\
	    ((xt_ft) ((addr_ft)(nm) + NAME_SIZE(NAME_LENGTH(nm))))

typedef struct name_header *	name_p;

struct name_header {
    name_p		prev;
    char_ft		flags;
    char_ft		ident[NAME_MAX_LENGTH];
};

extern name_p lookup(vmstate_p, c_addr_ft, cell_ft);
extern name_p addname(vmstate_p, c_addr_ft, cell_ft, vminstr_fn);
extern void linkname(name_p);

#define MAX_SEARCH_ORDER	8


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
	name_p		forth_wordlist;	/* FORTH-WORDLIST */
	name_p *	current;	/* CURRENT */
	cell_ft		n_search_order;
	name_p *	search_order[MAX_SEARCH_ORDER];

	definition_d	literal_instr;	/* for LITERAL runtime xt */
	definition_d	postpone_instr;	/* for POSTPONE non-immediate */
	definition_d	skip_instr;	/* for ELSE runtime xt */
	definition_d	fskip_instr;	/* for IF runtime xt */
	definition_d	do_instr;	/* for DO runtime xt */
	definition_d	plus_loop_instr; /* for +LOOP runtime xt */
	definition_d	does_instr;	/* for DOES> runtime xt */
	definition_d	s_quote_instr;	/* for S" runtime xt */

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
#define DO_POSTPONE_XT		(&DICT.postpone_instr)
#define SKIP_XT			(&DICT.skip_instr)
#define FSKIP_XT		(&DICT.fskip_instr)
#define DO_DO_XT		(&DICT.do_instr)
#define PLUS_LOOP_XT		(&DICT.plus_loop_instr)
#define DOES_XT			(&DICT.does_instr)
#define S_QUOTE_XT		(&DICT.s_quote_instr)

extern addr_ft allot(vmstate_p, cell_ft);

typedef struct defn *defn_data_p;

typedef void (*defn_fn)(vmstate_p, defn_data_p);

typedef struct defn {
    defn_fn		fn;
    void *		data0;
    void *		data1;
    cell_ft		flags;
} defn_dt;

extern defn_dt arithops_defns[];
extern defn_dt control_defns[];
extern defn_dt dictionary_defns[];
extern defn_dt interpret_defns[];
extern defn_dt memops_defns[];
extern defn_dt multops_defns[];
extern defn_dt names_defns[];
extern defn_dt stackops_defns[];
extern defn_dt termio_defns[];
extern defn_dt fileops_defns[];

extern void define_name(vmstate_p, defn_data_p);
extern void compile_name(vmstate_p, defn_data_p);


/*
 */

extern cell_ft parse(char_ft, c_addr_ft, cell_ft);
extern c_addr_ft parse_name(cell_ft *);

extern void compile_literal(vmstate_p, cell_ft);
extern void compile_xt(vmstate_p, xt_ft);
extern vminstr_p compile_skip(vmstate_p, xt_ft);
extern void patch(vminstr_p, vminstr_p);
extern void quit(vmstate_p, FILE *);
extern void interpret_string(vmstate_p, char *);

#define COMMA(vm, x)	(*(a_addr_ft)allot((vm), CELL_SIZE) = (cell_ft) (x))
#define ALIGN(vm)	(DICT.here = ALIGNED(DICT.here))

extern char initialize_forth[];

/*
 */

struct options {
    bool	is_interactive;
    char *	startup_file;
    int		argc;
    char **	argv;
};

extern struct options forth_options;

#define IS_INTERACTIVE(input)	(is_interactive((input), &forth_options))

extern void process_args(int, char *[], struct options *);
extern bool is_interactive(FILE *, struct options *);

#endif
