/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "forth.h"

/*
 * vm.c - Top-most layer of dictionary initialization, and VM main
 *   execution loop.
 */


static
char *forth_exceptions[] =
{
/*  -1 */	NULL,	/* ABORT */
/*  -2 */	NULL,	/* ABORT" */
/*  -3 */	"stack overflow",
/*  -4 */	"stack underflow",
/*  -5 */	"return stack overflow",
/*  -6 */	"return stack underflow",
/*  -7 */	"do-loops nested too deeply during execution",
/*  -8 */	"dictionary overflow",
/*  -9 */	"invalid memory address",
/* -10 */	"division by zero",
/* -11 */	"result out of range",
/* -12 */	"argument type mismatch",
/* -13 */	"undefined word",
/* -14 */	"interpreting a compile-only word",
/* -15 */	"invalid FORGET",
/* -16 */	"attempt to use zero-length string as a name",
/* -17 */	"pictured numeric output string overflow",
/* -18 */	"parsed string overflow",
/* -19 */	"definition name too long",
/* -20 */	"write to a read-only location",
/* -21 */	"unsupported operation (e.g., AT-XY on a too-dumb terminal)",
/* -22 */	"control structure mismatch",
/* -23 */	"address alignment exception",
/* -24 */	"invalid numeric argument",
/* -25 */	"return stack imbalance",
/* -26 */	"loop parameters unavailable",
/* -27 */	"invalid recursion",
/* -28 */	"user interrupt",
/* -29 */	"compiler nesting",
/* -30 */	"obsolescent feature",
/* -31 */	">BODY used on non-CREATEd definition",
/* -32 */	"invalid name argument (e.g., TO xxx)",
/* -33 */	"block read exception",
/* -34 */	"block write exception",
/* -35 */	"invalid block number",
/* -36 */	"invalid file position",
/* -37 */	"file I/O exception",
/* -38 */	"non-existent file",
/* -39 */	"unexpected end of file",
/* -40 */	"invalid BASE for floating point conversion",
/* -41 */	"loss of precision",
/* -42 */	"floating-point divide by zero",
/* -43 */	"floating-point result out of range",
/* -44 */	"floating-point stack overflow",
/* -45 */	"floating-point stack underflow",
/* -46 */	"floating-point invalid argument",
/* -47 */	"compilation word list deleted",
/* -48 */	"invalid POSTPONE",
/* -49 */	"search-order overflow",
/* -50 */	"search-order underflow",
/* -51 */	"compilation word list changed",
/* -52 */	"control-flow stack overflow",
/* -53 */	"exception stack overflow",
/* -54 */	"floating-point underflow",
/* -55 */	"floating-point unidentified fault",
/* -56 */	NULL,	/* QUIT */
/* -57 */	"exception in sending or receiving a character",
/* -58 */	"[IF], [ELSE], or [THEN] exception",
};


static void
handle_exception(int throwcode, vmstate_p vm)
{
    char *excdesc = forth_exceptions[-throwcode-1];

    if (excdesc != NULL) {
	CLEAR_RSTACK(vm);
	CLEAR_STACK(vm);
	fprintf(stderr, "%s\n", excdesc);
    }
}


static void
execute(vmstate_p vm, xt_ft entry_xt)
{
    vminstr_p ip = entry_xt->handler(NULL, vm, entry_xt[1].arg);

    while (ip != NULL) {
	xt_ft xtok = ip->xtok;
	ip = xtok->handler(ip + 1, vm, xtok[1].arg);
    }
}


static a_addr_ft
get_state(vmstate_p vm)
{
    int depth = DEPTH(vm);
    execute(vm, NAME_XT(lookup("STATE", sizeof ("STATE") - 1)));
    assert(DEPTH(vm) == depth + 1);
    return (a_addr_ft) POP(vm);
}


vminstr_p
interpret_names(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    a_addr_ft state_ptr = get_state(vm);

    while (ip->str != NULL) {
	name_p name = lookup(ip->str, strlen(ip->str));
	if (name == NULL) {
	    fprintf(stderr, "name %s not found\n", ip->str);
	    abort();
	}
	xt_ft xtok = NAME_XT(name);
	if (*state_ptr && !NAME_IS_IMMEDIATE(name)) {
	    comma((cell_ft) xtok);
	} else if (!*state_ptr && !NAME_IS_INTERPRETABLE(name)) {
	    fprintf(stderr,
		    "encountered %s in interpretion state\n", ip->str);
	    abort();
	} else {
	    execute(vm, xtok);
	}
	ip++;
    }
    return ip + 1;
}


vminstr_p
i_postpone(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    assert(*get_state(vm));

    name_p name = lookup(ip->str, strlen(ip->str));
    if (name == NULL) {
	fprintf(stderr, "name %s not found for postpone\n", ip->str);
	abort();
    }
    assert(NAME_IS_IMMEDIATE(name));
    comma((cell_ft) NAME_XT(name));

    return interpret_names(ip + 1, vm, ignore);
}


vminstr_p
i_literal(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    a_addr_ft state_ptr = get_state(vm);

    cell_ft n = ip[0].cell;
    if (*state_ptr) {
	comma((cell_ft) DO_LITERAL);
	comma(n);
    } else {
	PUSH(vm, n);
    }

    return interpret_names(ip + 1, vm, ignore);
}


vminstr_p
i_call(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_RPUSH(vm, 1);
    RPUSH(vm, ip + 1);
    return ip->ip;
}


static
META_FORTH(test_forth) // {
    XCOLON("CHECKSTACK")
	S("DEPTH") BEGIN DUP WHILE
	    SWAP DROP L('!') EMIT L(1) MINUS
	REPEAT DROP
    XSEMICOLON
    XCOLON("REPORT")
	IF L('P') ELSE L('F') THEN EMIT
    XSEMICOLON
    XCOLON("ASSERT-TRUE")
	S("REPORT") S("CHECKSTACK")
    XSEMICOLON
    XCOLON("ASSERT-FALSE")
	L(0) EQUALS S("REPORT") S("CHECKSTACK")
    XSEMICOLON

    // sanity checks; if these can't pass, nothing is reliable
    L(0) S("CHECKSTACK") S("CHECKSTACK")
    L(F_TRUE) S("ASSERT-TRUE")
    L(F_FALSE) S("ASSERT-FALSE")

    XCOLON("TEST-IF") IF L(F_TRUE) THEN XSEMICOLON
    L(F_TRUE) S("TEST-IF") S("ASSERT-TRUE")
    L(1) S("TEST-IF") S("ASSERT-TRUE")
    L(F_FALSE) S("TEST-IF") S("CHECKSTACK")

    XCOLON("TEST-ELSE") IF L(F_FALSE) ELSE L(F_TRUE) THEN XSEMICOLON
    L(F_TRUE) S("TEST-ELSE") S("ASSERT-FALSE")
    L(F_FALSE) S("TEST-ELSE") S("ASSERT-TRUE")
    CR

    // XXX arith prim tests
    // XXX stack prim tests
    // XXX memory prim tests
    // XXX mult prim tests
    // compile prim tests - everywhere you look
    // terminal prim tests - can't automate

    // memory ops tests
    L('@') EMIT
    L(1) CELLS L(0) CELL_PLUS EQUALS S("ASSERT-TRUE")
    L(1) CELLS L(1) ALIGNED EQUALS S("ASSERT-TRUE")
    L(1) CHARS L(0) CHAR_PLUS EQUALS S("ASSERT-TRUE")
    // XXX test +!
    CR

    // dictionary ops tests
    L(',') EMIT
    HERE HERE ALIGNED EQUALS S("ASSERT-TRUE")
    HERE L(1) ALLOT L(-1) ALLOT HERE EQUALS S("ASSERT-TRUE")
    XCOLON("TEST-ALLOT")
	/* ( here count -- ) */
	OVER TO_R PLUS HERE EQUALS R_FROM HERE MINUS ALLOT
	S("ASSERT-TRUE")
    XSEMICOLON
    HERE L(1) CELLS DUP ALLOT S("TEST-ALLOT")
    HERE L(1) CHARS DUP ALLOT S("TEST-ALLOT")
    HERE L(1) ALLOT ALIGN L(1) CELLS S("TEST-ALLOT")
    HERE L(3) COMMA L(1) CELLS S("TEST-ALLOT")
    L(3) HERE FETCH EQUALS S("ASSERT-TRUE")
    HERE L('A') C_COMMA L(1) CHARS S("TEST-ALLOT")
    L('A') HERE C_FETCH EQUALS S("ASSERT-TRUE")
    CR

    // compile ops tests - everywhere you look
    // control ops tests - mostly covered

    // arith ops
    L('+') EMIT
    L(-1) ABS L(1) EQUALS S("ASSERT-TRUE")
    L(1) ABS L(1) EQUALS S("ASSERT-TRUE")
    L(0) ABS S("ASSERT-FALSE")
    CR

    // format ops
    L('#') EMIT
    L(2) BASE STORE DECIMAL BASE FETCH L(10) EQUALS S("ASSERT-TRUE")
    HEX BASE FETCH L(16) EQUALS S("ASSERT-TRUE")
    DECIMAL

    XCOLON("CHECK-HOLD-STRING")
	/* ( char ... c-addr u -- ) */
	S("DEPTH") L(2) MINUS EQUALS DUP S("REPORT")
	L('<') EMIT IF
	    TO_R BEGIN S("DEPTH") WHILE
		R_FROM DUP CHAR_PLUS TO_R
		C_FETCH EQUALS S("REPORT")
	    REPEAT R_FROM
	THEN DROP S("CHECKSTACK") L('>') EMIT
    XSEMICOLON

    LESS_NUMBER S("CHECKSTACK") L(0) L(0) NUMBER_GREATER
	S("CHECK-HOLD-STRING")

    L('A') LESS_NUMBER DUP HOLD L(0) L(0) NUMBER_GREATER
	S("CHECK-HOLD-STRING")

    XCOLON("TEST-SIGN")
	LESS_NUMBER SIGN L(0) L(0) NUMBER_GREATER
	S("CHECK-HOLD-STRING")
    XSEMICOLON
    L(1) S("TEST-SIGN")
    L(0) S("TEST-SIGN")
    L('-') L(-1) S("TEST-SIGN")

    XCOLON("CHECK-#")
	LESS_NUMBER NUMBER_SIGN NUMBER_GREATER
	S("CHECK-HOLD-STRING")
    XSEMICOLON

    XCOLON("CHECK-BASE-#")
	BASE FETCH TO_R BASE STORE CR L('#') EMIT L('(') EMIT
	// single digit test
	L('0') L(0) L(0) S("CHECK-#")
	L('1') L(1) L(0) S("CHECK-#")
	BASE FETCH L(1) MINUS DUP DUP
	    L(10) LESS IF L('0') ELSE L('A'-10) THEN PLUS
	SWAP L(0) S("CHECK-#")
	// two digit test
	L('0') BASE FETCH L(0) S("CHECK-#")
	L('1') BASE FETCH L(1) PLUS L(0) S("CHECK-#")
	R_FROM BASE STORE
	L(')') EMIT
    XSEMICOLON
    L(2) S("CHECK-BASE-#")
    L(10) S("CHECK-BASE-#")
    L(36) S("CHECK-BASE-#")

    // 0
    // 1
    // BASE-1
    // BASE
    // BASE+1
    XCOLON("CHECK-#S")
	LESS_NUMBER NUMBER_S NUMBER_GREATER
	S("CHECK-HOLD-STRING")
    XSEMICOLON

    XCOLON("CHECK-BASE-#S")
	BASE FETCH TO_R BASE STORE CR L('#') EMIT L('(') EMIT
	// single digit test
	L('0') L(0) L(0) S("CHECK-#S")
	L('1') L(1) L(0) S("CHECK-#S")
	BASE FETCH L(1) MINUS DUP DUP
	    L(10) LESS IF L('0') ELSE L('A'-10) THEN PLUS
	SWAP L(0) S("CHECK-#S")
	// two digit test
	L('0') L('1') BASE FETCH L(0) S("CHECK-#S")
	L('1') L('1') BASE FETCH L(1) PLUS L(0) S("CHECK-#S")
	R_FROM BASE STORE
	L(')') EMIT
    XSEMICOLON
    L(2) S("CHECK-BASE-#S")
    L(10) S("CHECK-BASE-#S")
    L(36) S("CHECK-BASE-#S")
    CR
END_META // }


static
META_FORTH(dump_dictionary) // {
    XCOLON("COUNT") DUP CHAR_PLUS SWAP C_FETCH XSEMICOLON
    XCOLON("ADDR.")
	/* ( addr -- ) */
	BASE FETCH SWAP HEX
	L(0) LESS_NUMBER L(2 * CELL_SIZE) BEGIN
	    TO_R NUMBER_SIGN R_FROM
	    L(1) MINUS DUP L(0) EQUALS
	UNTIL DROP NUMBER_GREATER TYPE
	BASE STORE
    XSEMICOLON
    XCOLON("DUMP-WORDLIST")
	/* ( wid -- ) */
	BASE FETCH SWAP HEX
	BEGIN FETCH DUP WHILE
	    DUP
		S("ADDR.")
	    DUP CELL_PLUS COUNT DUP L(~0x1f) AND TO_R L(0x1f) AND
	    OVER OVER CHARS PLUS ALIGNED FETCH
		SPACE SPACE S("ADDR.")
	    R_FROM L(0) LESS_NUMBER NUMBER_SIGN NUMBER_SIGN NUMBER_GREATER
		SPACE TYPE
		SPACE TYPE CR
	REPEAT DROP
	BASE STORE
    XSEMICOLON
    XCOLON("DUMP-DICTIONARY")
	HERE S("ADDR.") CR
	L(&DICT.forth_wordlist) S("DUMP-WORDLIST")
	L(DICT_START) S("ADDR.") CR
	L(DICT_END) L(DICT_START) MINUS DOT
	HERE L(DICT_START) MINUS DOT
	L(DICT_END) HERE MINUS DOT CR
    XSEMICOLON
END_META // }


static
DIRECT_FORTH(initialize) // {
    X(init_dictionary)
    X(init_names)
    CALL(init_arith_prim)
    CALL(init_stack_prim)
    CALL(init_memory_prim)
    CALL(init_mult_prim)
    CALL(init_compile_prim)
    CALL(init_terminal_prim)
    CALL(init_memory_ops)
    CALL(init_dictionary_ops)
    CALL(init_compile_ops)
    CALL(init_control_ops)
    CALL(init_arith_ops)
    CALL(init_parse_ops)
    CALL(init_format_ops)
    CALL(init_terminal_ops)
    CALL(test_forth)
    CALL(dump_dictionary)
END_DIRECT // }


void
init_forth(vmstate_p vm)
{
    CLEAR_STACK(vm);
    CLEAR_RSTACK(vm);

    volatile int	throwcode;
    if ((throwcode = setjmp(vm->interp_loop)) != 0) {
	handle_exception(throwcode, vm);
	abort();
    }

    RPUSH(vm, NULL);
    vminstr_p ip = initialize;
    while (ip != NULL) {
	ip = ip->handler(ip + 1, vm, NULL);
    }
}


void
interpret(vmstate_p start_vm, char *name)
{
    name_p entry = lookup(name, strlen(name));
    if (entry == NULL) {
	return;
    }

    volatile vmstate_p  vm = start_vm;
    volatile xt_ft      xtok = NAME_XT(entry);
    volatile int	throwcode;
    while ((throwcode = setjmp(vm->interp_loop)) != 0) {
	handle_exception(throwcode, vm);
    }
    execute(vm, xtok);
}
