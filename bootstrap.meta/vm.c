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
	} else if (*state_ptr || NAME_IS_INTERPRETABLE(name)) {
	    execute(vm, xtok);
	} else {
	    fprintf(stderr,
		    "encountered %s in interpretion state\n", ip->str);
	    abort();
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
DIRECT_FORTH(initialize) // {
    CALL(init_dictionary)
    CALL(init_names)
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
    CALL(init_format_ops)
    CALL(init_terminal_ops)
    CALL(init_parse_ops)
    CALL(init_interpreter_ops)
    CALL(init_name_ops)
#ifdef TEST
    CALL(test_forth)
#endif
    META QUIT DIRECT
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
