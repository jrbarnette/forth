/*
 * Copyright 2007, by J. Richard Barnette
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
 * 3.1.
 *
 * addr-unit:	Smallest addressable unit of storage
 * char:	Character value (superset of ASCII)
 * cell:	Value on the data stack (or any other stack)
 * addr:	Cell-sized value of an address
 * a-addr:	Address aligned to point to a valid cell address
 * c-addr:	Address aligned to point to a valid character address
 * dcell:	Two cells, laid out as required by section 3.1.4
 * string:	( c-addr u ) pair designating a string address and length
 */

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

#define CELL_SIZE	(sizeof (cell_ft))
#define CELL_ALIGNMENT	(sizeof (cell_ft))
#define ALIGNED(n)	(((n) + CELL_ALIGNMENT - 1) & -CELL_ALIGNMENT)
#define CELLS		* CELL_SIZE


/*
 * VM specific definitions not directly visible to Forth code
 */

#define STACK_SIZE	2048
#define RSTACK_SIZE	64

typedef struct vminstr *	xt_ft;
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

struct vminstr {
    vminstr_fn		handler;
    addr_unit_ft	data[1];
};

extern void overflow(vmstate_p);
extern void underflow(vmstate_p);

#define CHECK_POP(vm, n)	\
	    (((vm)->sp + (n) < STACK_SIZE) || (underflow(vm), 0))
#define CHECK_PUSH(vm, n)	\
	    (((vm)->sp >= (n)) || (overflow(vm), 0))
#define CHECK_RPOP(vm, n)	\
	    (((vm)->rsp + (n) < RSTACK_SIZE) || (underflow(vm), 0))
#define CHECK_RPUSH(vm, n)	\
	    (((vm)->rsp >= (n)) || (overflow(vm), 0))

#define CLEAR_STACK(vm)		((vm)->sp = STACK_SIZE)
#define CLEAR_RSTACK(vm)	((vm)->rsp = RSTACK_SIZE)

#define POP(vm)		((vm)->stack[(vm)->sp++])
#define PUSH(vm, c)	((vm)->stack[--(vm)->sp] = (cell_ft)(c))
#define RPOP(vm)	((vm)->rstack[(vm)->rsp++])
#define RPUSH(vm, c)	((vm)->rstack[--(vm)->rsp] = (cell_ft)(c))

extern void interpret(vmstate_p, xt_ft);


/*
 * Definitions for names in the dictionary; these are visible to
 * Forth code, but the details are implementation dependent
 */

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

extern name_p lookup(char *, size_t);
extern name_p addname(char *, vminstr_fn);


/*
 * C definitions and declarations relating to the Forth dictionary,
 * including structures for initialization
 */

#define DICTIONARY_SIZE		0x10000

extern union dict {
    struct {
	cell_ft		here;
	name_p		namelist;

	cell_ft		base;
    } dict_static_data;
    addr_unit_ft	dict_space[DICTIONARY_SIZE];
} dictionary;

#define DICT_STATIC(field)	

#define dict_here	dictionary.dict_static_data.here
#define dict_namelist	dictionary.dict_static_data.namelist
#define dict_base	dictionary.dict_static_data.base

extern addr_ft allot(cell_ft);
extern void immediate(void);

typedef void (*definer_fn)(name_p, void *);

typedef struct defn {
    char *		name;
    vminstr_fn		fn;
    definer_fn		definer;
    void *		datap;
} defn_dt;

extern defn_dt primitive_defns[];
extern defn_dt dictionary_defns[];
extern defn_dt compile_defns[];

extern void init_forth(void);

#endif
