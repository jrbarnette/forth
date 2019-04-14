/*
 * Copyright 2015, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef FORTH_H
#define FORTH_H

#include <setjmp.h>
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
 * Initialization and direct threaded interpretation primitives.
 */

/* direct threaded name definition primitives */
extern vminstr_p i_startname(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p i_addname(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p i_setflags(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p i_linkname(vminstr_p, vmstate_p, vmarg_p);

/* vm execution and meta interpretation primitives */
extern vminstr_p meta_interpret(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p meta_compile(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p i_lookup(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_literal(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_s_quote(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_c_quote(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_colon(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_constant(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_create(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_variable(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_execute(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_exit(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_throw(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_clear(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_rclear(vminstr_p, vmstate_p, vmarg_p);

/* dictionary primitives */
extern vminstr_p x_comma(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_align(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_allot(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_c_comma(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_here(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_unused(vminstr_p, vmstate_p, vmarg_p);

/* stack primitives */
extern vminstr_p x_to_r(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_question_dup(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_depth(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_drop(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_dup(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_over(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_r_from(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_r_fetch(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_rot(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_swap(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_two_to_r(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_two_r_from(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_two_r_fetch(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_pick(vminstr_p, vmstate_p, vmarg_p);

/* arithmetic and logical primitives */
extern vminstr_p x_plus(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_minus(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_two_star(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_two_slash(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_less_than(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_equals(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_greater_than(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_and(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_invert(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_lshift(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_negate(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_or(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_rshift(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_u_less(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_xor(vminstr_p, vmstate_p, vmarg_p);

/* memory primitives */
extern vminstr_p x_store(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_fetch(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_c_store(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_c_fetch(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_fill(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_move(vminstr_p, vmstate_p, vmarg_p);

/* multiple cell multiply and divide primitives */
extern vminstr_p x_star(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_slash(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_slash_mod(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_m_star(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_mod(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_u_m_star(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_u_m_slash_mod(vminstr_p, vmstate_p, vmarg_p);

/* terminal I/O primitives */
extern vminstr_p x_emit(vminstr_p, vmstate_p, vmarg_p);

/* control flow primitives */
extern vminstr_p do_skip(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_fskip(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_plus_loop(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_do(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_i(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_j(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_unloop(vminstr_p, vmstate_p, vmarg_p);

/* parse primitives */
extern vminstr_p x_refill_terminal(vminstr_p, vmstate_p, vmarg_p);

/* file I/O primitives */
extern vminstr_p x_bin(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_close_file(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_create_file(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_file_position(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_file_size(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_open_file(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_r_o(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_r_w(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_read_file(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_read_line(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_reposition_file(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_w_o(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_write_file(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_write_line(vminstr_p, vmstate_p, vmarg_p);

#endif // FORTH_H
