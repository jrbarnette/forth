/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
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
    vminstr_fn		handler;
    vminstr_p		ip;
    xt_ft		xtok;
    cell_ft		cell;
    snumber_ft		offset;
    char_ft		cdata[1];
    char *		str;
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

#define CAN_PUSH(vm, n)		((vm)->sp >= (vm)->stack + (n))
#define CAN_POP(vm, n)		((vm)->sp <= (vm)->stack + STACK_SIZE - (n))
#define CAN_RPUSH(vm, n)	((vm)->rsp >= (vm)->rstack + (n))
#define CAN_RPOP(vm, n)		((vm)->rsp <= (vm)->rstack + RSTACK_SIZE - (n))

#define CHECK_PUSH(vm, n)	CHECK(vm, CAN_PUSH(vm, n), -3)
#define CHECK_POP(vm, n)	CHECK(vm, CAN_POP(vm, n), -4)
#define CHECK_RPUSH(vm, n)	CHECK(vm, CAN_RPUSH(vm, n), -5)
#define CHECK_RPOP(vm, n)	CHECK(vm, CAN_RPOP(vm, n), -6)

#define POP(vm)			(*(vm)->sp++)
#define PUSH(vm, c)		(*--(vm)->sp = (cell_ft)(c))
#define RPOP(vm)		(*(vm)->rsp++)
#define RPUSH(vm, c)		(*--(vm)->rsp = (cell_ft)(c))


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


/*
 * C definitions and declarations relating to the Forth dictionary,
 * including structures for initialization.
 */

#define DICTIONARY_SIZE		(0x10000 XCELLS)

extern union dict {
    struct {
	addr_ft		here;		/* HERE */
	name_p		forth_wordlist;	/* FORTH-WORDLIST */

	definition_d	literal_instr;	/* for LITERAL runtime xt */
    } static_data;
    addr_unit_ft	space[DICTIONARY_SIZE];
} dictionary;

#define DICT		dictionary.static_data
#define DICT_START	(&dictionary.space[0])
#define DICT_END	(&dictionary.space[DICTIONARY_SIZE])

#define DO_LITERAL	(&DICT.literal_instr)


/*
 */

extern void init_forth(vmstate_p);

extern void align(void);
extern addr_ft allot(size_t);
extern void comma(cell_ft);
extern name_p lookup(char *, size_t);

extern vminstr_p init_dictionary(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p init_names(vminstr_p, vmstate_p, vmarg_p);

extern vminstr_d init_stack_prim[];
extern vminstr_d init_arith_prim[];
extern vminstr_d init_memory_prim[];
extern vminstr_d init_compile_prim[];

extern vminstr_d init_memory_ops[];
extern vminstr_d init_dictionary_ops[];
extern vminstr_d init_compile_ops[];
extern vminstr_d init_control_ops[];


/*
 */

extern vminstr_p i_call(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p i_comma(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p i_addname(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p i_setflags(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p i_postpone(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p i_literal(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p interpret_names(vminstr_p, vmstate_p, vmarg_p);

extern vminstr_p x_exit(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_colon(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_variable(vminstr_p, vmstate_p, vmarg_p);

#define DIRECT_FORTH(nm) vminstr_d nm[] = {
#define END_DIRECT	X(x_exit) };
#define META_FORTH(nm)	DIRECT_FORTH(nm) META
#define END_META	DIRECT END_DIRECT

#define X(x)		{ .handler = x },
#define S(s)		{ .str = s },
#define XCOMMA(n)	X(i_comma) { .cell = (cell_ft) (n) },
#define CALL(x)		X(i_call) { .ip = (x) },
#define PRIM(nm, hdlr)	X(i_addname) S(nm) { .handler = hdlr },
#define FLAGS(f)	X(i_setflags) { .cell = NAME_TYPE_ ## f },

#define META		X(interpret_names)
#define ML(n)		X(i_literal) { .cell = (cell_ft) (n) },
#define DIRECT		S(NULL)

#define L(n)		DIRECT ML(n)
#define XIMMEDIATE	DIRECT FLAGS(IMMEDIATE) META
#define NO_INTERPRET	DIRECT FLAGS(NO_INTERPRET) META
#define COMPILE_ONLY	DIRECT FLAGS(COMPILE) META
#define NF_IMMEDIATE	L(NAME_TYPE_IMMEDIATE)
#define NF_NO_INTERPRET	L(NAME_TYPE_NO_INTERPRET)

#define ANON(h)		HERE DIRECT XCOMMA(h) META
#define XNONAME		ANON(do_colon) S("]")
#define XVARIABLE(nm)	DIRECT PRIM(nm, do_variable) ML(CELL_SIZE) ALLOT
#define XCOLON(nm)	DIRECT PRIM(nm, do_colon) META S("]")
#define XSEMICOLON	EXIT S("[")
#define XPOSTPONE	DIRECT X(i_postpone)
#define INTERP(code)	LEFT_BRACKET code RIGHT_BRACKET

#define STORE            S("!")
#define NUMBER_SIGN      S("#")
#define NUMBER_GREATER   S("#>")
#define NUMBER_S         S("#S")
#define TICK             S("'")
#define PAREN            S("(")
#define STAR             S("*")
#define STAR_SLASH       S("*/")
#define STAR_SLASH_MOD   S("*/MOD")
#define PLUS_STORE       S("+!")
#define PLUS             S("+")
#define PLUS_LOOP        S("+LOOP")
#define COMMA            S(",")
#define MINUS            S("-")
#define DOT              S(".")
#define DOT_PAREN        S(".(")
#define DOT_R            S(".R")
#define DOT_QUOTE        S(".\"")
#define SLASH            S("/")
#define SLASH_MOD        S("/MOD")
#define ZERO_LESS        S("0<")
#define ZERO_NOT_EQUALS  S("0<>")
#define ZERO_EQUALS      S("0=")
#define ZERO_GREATER     S("0>")
#define ONE_PLUS         S("1+")
#define ONE_MINUS        S("1-")
#define TWO_STORE        S("2!")
#define TWO_STAR         S("2*")
#define TWO_SLASH        S("2/")
#define TWO_TO_R         S("2>R")
#define TWO_FETCH        S("2@")
#define TWO_DROP         S("2DROP")
#define TWO_DUP          S("2DUP")
#define TWO_OVER         S("2OVER")
#define TWO_R_FROM       S("2R>")
#define TWO_R_FETCH      S("2R@")
#define TWO_SWAP         S("2SWAP")
#define COLON            S(":")
#define COLON_NONAME     S(":NONAME")
#define SEMICOLON        S(";")
#define LESS             S("<")
#define LESS_NUMBER      S("<#")
#define NOT_EQUALS       S("<>")
#define EQUALS           S("=")
#define GREATER          S(">")
#define TO_BODY          S(">BODY")
#define TO_IN            S(">IN")
#define TO_NUMBER        S(">NUMBER")
#define TO_R             S(">R")
#define Q_DO             S("?DO")
#define Q_DUP            S("?DUP")
#define FETCH            S("@")
#define ABORT            S("ABORT")
#define ABORT_QUOTE      S("ABORT\"")
#define ABS              S("ABS")
#define ACCEPT           S("ACCEPT")
#define AGAIN            S("AGAIN")
#define ALIGN            S("ALIGN")
#define ALIGNED          S("ALIGNED")
#define ALLOT            S("ALLOT")
#define AND              S("AND")
#define BASE             S("BASE")
#define BEGIN            S("BEGIN")
#define BL               S("BL")
#define C_STORE          S("C!")
#define C_COMMA          S("C,")
#define C_FETCH          S("C@")
#define CASE             S("CASE")
#define CELL_PLUS        S("CELL+")
#define CELLS            S("CELLS")
#define CHAR             S("CHAR")
#define CHAR_PLUS        S("CHAR+")
#define CHARS            S("CHARS")
#define COMPILE_COMMA    S("COMPILE,")
#define CONSTANT         S("CONSTANT")
#define COUNT            S("COUNT")
#define CR               S("CR")
#define CREATE           S("CREATE")
#define C_QUOTE          S("C\"")
#define DECIMAL          S("DECIMAL")
//#define DEPTH            S("DEPTH")
#define DO               S("DO")
#define DOES             S("DOES>")
#define DROP             S("DROP")
#define DUP              S("DUP")
#define ELSE             S("ELSE")
#define EMIT             S("EMIT")
#define ENDCASE          S("ENDCASE")
#define ENDOF            S("ENDOF")
#define ENVIRONMENT_Q    S("ENVIRONMENT?")
#define ERASE            S("ERASE")
#define EVALUATE         S("EVALUATE")
#define EXECUTE          S("EXECUTE")
#define EXIT             S("EXIT")
#define FALSE            S("FALSE")
#define FILL             S("FILL")
#define FIND             S("FIND")
#define FM_SLASH_MOD     S("FM/MOD")
#define HERE             S("HERE")
#define HEX              S("HEX")
#define HOLD             S("HOLD")
#define I                S("I")
#define IF               S("IF")
#define IMMEDIATE        S("IMMEDIATE")
#define INVERT           S("INVERT")
#define J                S("J")
#define KEY              S("KEY")
#define LEAVE            S("LEAVE")
#define LITERAL          S("LITERAL")
#define LOOP             S("LOOP")
#define LSHIFT           S("LSHIFT")
#define M_STAR           S("M*")
#define MARKER           S("MARKER")
#define MAX              S("MAX")
#define MIN              S("MIN")
#define MOD              S("MOD")
#define MOVE             S("MOVE")
#define NEGATE           S("NEGATE")
#define NIP              S("NIP")
#define OF               S("OF")
#define OR               S("OR")
#define OVER             S("OVER")
#define PAD              S("PAD")
#define PARSE            S("PARSE")
//#define PICK             S("PICK")
#define POSTPONE         S("POSTPONE")
#define QUIT             S("QUIT")
#define R_FROM           S("R>")
#define R_FETCH          S("R@")
#define RECURSE          S("RECURSE")
#define REFILL           S("REFILL")
#define REPEAT           S("REPEAT")
#define RESTORE_INPUT    S("RESTORE-INPUT")
#define ROLL             S("ROLL")
#define ROT              S("ROT")
#define RSHIFT           S("RSHIFT")
#define S_TO_D           S("S>D")
#define SAVE_INPUT       S("SAVE-INPUT")
#define SIGN             S("SIGN")
#define SM_SLASH_REM     S("SM/REM")
#define SOURCE           S("SOURCE")
#define SOURCE_ID        S("SOURCE-ID")
#define SPACE            S("SPACE")
#define SPACES           S("SPACES")
#define STATE            S("STATE")
#define SWAP             S("SWAP")
#define S_QUOTE          S("S\"")
#define THEN             S("THEN")
#define TO               S("TO")
#define TRUE             S("TRUE")
#define TUCK             S("TUCK")
#define TYPE             S("TYPE")
#define U_DOT            S("U.")
#define U_DOT_R          S("U.R")
#define U_LESS           S("U<")
#define U_GREATER        S("U>")
#define UM_STAR          S("UM*")
#define UM_SLASH_MOD     S("UM/MOD")
#define UNLOOP           S("UNLOOP")
#define UNTIL            S("UNTIL")
#define UNUSED           S("UNUSED")
#define VALUE            S("VALUE")
#define VARIABLE         S("VARIABLE")
#define WHILE            S("WHILE")
#define WITHIN           S("WITHIN")
#define WORD             S("WORD")
#define XOR              S("XOR")
#define LEFT_BRACKET     S("[")
#define BRACKET_TICK     S("[']")
#define BRACKET_CHAR     S("[CHAR]")
#define BRACKET_COMPILE  S("[COMPILE]")
#define BACKSLASH        S("\\")
#define RIGHT_BRACKET    S("]")


#endif
