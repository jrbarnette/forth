/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "forth.h"

/*
 * vm.c - Forth primitives and other C code for inner VM operation.
 */

// Contains all semantics for the following standard definitions:
//------  ------  ------  ------  ------  ------  ------  ------
// EXECUTE               6.1.1370 CORE                   38
// EXIT                  6.1.1380 CORE                   38
//
// THROW               9.6.1.2275 EXCEPTION              73
//------  ------  ------  ------  ------  ------  ------  ------

// Contains runtime or other partial semantics for the following:
//------  ------  ------  ------  ------  ------  ------  ------
// :                     6.1.0450 CORE                   30
// CONSTANT              6.1.0950 CORE                   35
// CREATE                6.1.1000 CORE                   36
// VARIABLE              6.1.2410 CORE                   47
//------  ------  ------  ------  ------  ------  ------  ------


void
execute(vmstate_p vm, xt_ft entry_xt)
{
    vminstr_p ip = entry_xt->handler(NULL, vm, entry_xt[1].arg);

    while (ip != NULL) {
	xt_ft xtok = ip->xtok;
	ip = xtok->handler(ip + 1, vm, xtok[1].arg);
    }
}


static xt_ft
meta_lookup(vmstate_p vm, vminstr_p ip)
{
    name_p name = lookup(vm, (c_addr_ft) ip->id, strlen(ip->id));
    if (name == NULL) {
	/*
	 * XXX ptrdiff_t wants %ld for 64-bit systems, but this
	 * could cause warnings on 32-bit systems.
	 */
	fprintf(stderr, "name '%s' not found at offset %ld\n",
		ip->id, ip - initialize_forth);
	abort();
    }
    return NAME_XT(name);
}


/* -------------------------------------------------------------- */

vminstr_p
meta_interpret(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    while (ip->id != NULL) {
	execute(vm, meta_lookup(vm, ip));
	ip++;
    }
    return ip + 1;
}


vminstr_p
meta_compile(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    while (ip->id != NULL) {
	COMPILE(vm, meta_lookup(vm, ip));
	ip++;
    }
    return ip + 1;
}


vminstr_p
i_lookup(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    PUSH(vm, meta_lookup(vm, ip));
    return ip + 1;
}


void
interpret_lines(vmstate_p vm, char **lines)
{
    char id[] = "EVALUATE";
    name_p name = lookup(vm, (c_addr_ft) id, sizeof (id) - 1);
    assert(name != NULL);
    xt_ft xt = NAME_XT(name);
    while (*lines != NULL) {
        char *s = *lines++;
	PUSH(vm, (cell_ft) s);
	PUSH(vm, (cell_ft) strlen(s));
	execute(vm, xt);
    }

    assert(DICT.state == STATE_INTERP);
    /* assert stacks are empty */
}


void
quit(vmstate_p vm, FILE *input)
{
    char id[] = "QUIT";
    name_p name = lookup(vm, (c_addr_ft) id, sizeof (id) - 1);
    assert(name != NULL);
    xt_ft xt = NAME_XT(name);
    DICT.lineno = 0;
    DICT.input = input;
    execute(vm, xt);
}


/* -------------------------------------------------------------- */

/* ( R: -- nest-sys ) initiation semantics */
vminstr_p
do_colon(vminstr_p ip, vmstate_p vm, vmarg_p newip)
{
    CHECK_RPUSH(vm, 1);
    RPUSH(vm, (cell_ft)ip);
    return newip->vminstrs;
}


/* ( -- x ) name execution semantics */
vminstr_p
do_constant(vminstr_p ip, vmstate_p vm, vmarg_p data_ptr)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, data_ptr->cell);
    return ip;
}


/* ( -- a-addr ) name execution semantics */
vminstr_p
do_create(vminstr_p ip, vmstate_p vm, vmarg_p data_ptr)
{
    vminstr_p does_ptr = data_ptr[0].ip;
    addr_ft body = data_ptr[1].data;

    CHECK_PUSH(vm, 1);
    CHECK_RPUSH(vm, 1);
    PUSH(vm, (cell_ft) body);
    RPUSH(vm, ip);
    return does_ptr;
}


/* ( -- a-addr ) name execution semantics */
vminstr_p
do_variable(vminstr_p ip, vmstate_p vm, vmarg_p var_addr)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) var_addr);
    return ip;
}


/* ( i*x xt -- i*j ) */
vminstr_p
x_execute(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    xt_ft xtok;

    CHECK_POP(vm, 1);
    xtok = (xt_ft)POP(vm);
    return xtok->handler(ip, vm, xtok[1].arg);
}


/* ( R: nest-sys -- ) execution semantics */
vminstr_p
x_exit(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_RPOP(vm, 1);
    return (vminstr_p)RPOP(vm);
}


vminstr_p
x_throw(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    cell_ft exc = POP(vm);
    if (exc != 0) {
	THROW(vm, exc);
    }
    return ip;
}


/* ( i*x -- ) */
vminstr_p
x_clear(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CLEAR_STACK(vm);
    return ip;
}


/* ( i*x -- ) */
vminstr_p
x_rclear(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CLEAR_RSTACK(vm);
    return ip;
}
