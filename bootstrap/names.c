/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "forth.h"
#include "direct.h"

/*
 * names.c - C internal functions and Forth standard words for
 *   dictionary name space.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// '                     6.1.0070 CORE                   25
// :                     6.1.0450 CORE                   30
// EXIT                  6.1.1380 CORE                   38
// ;                     6.1.0460 CORE                   30
// >BODY                 6.1.0550 CORE                   31
// CONSTANT              6.1.0950 CORE                   35
// CREATE                6.1.1000 CORE                   36
// VARIABLE              6.1.2410 CORE                   47
//
// FORTH-WORDLIST     16.6.1.1595 SEARCH                119
// SEARCH-WORDLIST    16.6.1.2192 SEARCH                120
//------  ------  ------  ------  ------  ------  ------  ------


/*
 * Look up a definition in a wordlist, and return a pointer to its
 * name header.  Return NULL if not found.
 */
static name_p
search_wordlist(name_p *names, c_addr_ft id, cell_ft len)
{
    name_p	cur;

    for (cur = *names; cur != NULL; cur = cur->prev) {
	int i;

	if (len != NAME_LENGTH(cur))
	    continue;

	i = 0;
	while (toupper(id[i]) == toupper(cur->ident[i])) {
	    i++;
	    if (i == len)
		return cur;
	}
    }

    return NULL;
}


/*
 * Look up a definition in the search order, and return a pointer to
 * its name header.  Return NULL if not found.
 */
name_p
lookup(vmstate_p vm, c_addr_ft id, cell_ft len)
{
    name_p nm = NULL;
    cell_ft i;

    if (len == 0)
	THROW(vm, -16);

    for (i = 0; i < DICT.n_search_order; i++) {
	nm = search_wordlist(DICT.search_order[i], id, len);
	if (nm != NULL) {
	    break;
	}
    }
    return nm;
}


/*
 * Routines for adding named definitions into the dictionary.
 */
static name_p
addname(vmstate_p vm, c_addr_ft id, cell_ft len, vminstr_fn handler)
{
    if (len == 0)		THROW(vm, -16);
    if (len > NAME_MAX_LENGTH)	THROW(vm, -19);

    XALIGN(vm);
    name_p name = (name_p) allot(vm, NAME_SIZE(len) + CELL_SIZE);
    name->prev = *DICT.current;
    name->flags = len;
    (void) memcpy(name->ident, id, len);

    xt_ft xtok = NAME_XT(name);
    xtok->handler = handler;
    assert(HERE == xtok[1].arg->data);

    return name;
}


static void
linkname(name_p name)
{
    *DICT.current = name;
}


/* -------------------------------------------------------------- */

vminstr_p
i_startname(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    char *id = (ip++)->id;
    cell_ft len = strlen(id);
    vminstr_fn handler = (ip++)->handler;
    name_p name = addname(vm, (c_addr_ft) id, len, handler);
    PUSH(vm, name);

    return ip;
}


vminstr_p
i_linkname(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    linkname((name_p) POP(vm));
    return ip;
}


vminstr_p
i_addname(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    char *id = (ip++)->id;
    cell_ft len = strlen(id);
    linkname(addname(vm, (c_addr_ft) id, len, (ip++)->handler));

    return ip;
}


vminstr_p
i_setflags(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    NAME_SET_TYPE(*DICT.current, ip->cell);
    return ip + 1;
}


/* -------------------------------------------------------------- */

/* ( "<spaces>name" -- xt ) */
vminstr_p
x_tick(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft len;
    c_addr_ft id = parse_name(&len);
    name_p nm = lookup(vm, id, len);

    if (nm == NULL)
	THROW(vm, -13);

    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) NAME_XT(nm));
    return ip;
}


/* ( R: -- nest-sys ) initiation semantics */
vminstr_p
do_colon(vminstr_p ip, vmstate_p vm, vmarg_p newip)
{
    CHECK_RPUSH(vm, 1);
    RPUSH(vm, (cell_ft)ip);
    return newip->vminstrs;
}


/* ( C: "<spaces>name" -- colon-sys ) */
vminstr_p
x_colon(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft len;
    c_addr_ft id = parse_name(&len);
    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) addname(vm, id, len, do_colon));
    DICT.state = STATE_COMPILE;
    return ip;
}


/* ( C: colon-sys -- ) compilation semantics */
vminstr_p
x_semicolon(vminstr_p ip, vmstate_p vm, vmarg_p exit_xt_ptr)
{
    CHECK_POP(vm, 1);
    COMPILE(vm, exit_xt_ptr->xtok);
    linkname((name_p) POP(vm));
    DICT.state = STATE_INTERP;
    return ip;
}


/* ( xt -- a-addr ) */
vminstr_p
x_to_body(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    xt_ft xtok;
    CHECK_POP(vm, 1);

    xtok = (xt_ft) POP(vm);
    if (xtok->handler != do_create) {
	THROW(vm, -31);
    }

    addr_ft body = xtok[2].arg->data;
    PUSH(vm, (cell_ft) body);
    return ip;
}


/* ( R: nest-sys -- ) execution semantics */
vminstr_p
x_exit(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_RPOP(vm, 1);
    return (vminstr_p)RPOP(vm);
}


/* ( -- x ) name execution semantics */
vminstr_p
do_constant(vminstr_p ip, vmstate_p vm, vmarg_p data_ptr)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, data_ptr->cell);
    return ip;
}


/* ( “<spaces>name” -- ) */
vminstr_p
x_constant(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft len;
    c_addr_ft id = parse_name(&len);

    CHECK_POP(vm, 1);
    linkname(addname(vm, id, len, do_constant));
    COMMA(vm, POP(vm));
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


/* ( “<spaces>name” -- ) */
vminstr_p
x_create(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft len;
    c_addr_ft id = parse_name(&len);
    linkname(addname(vm, id, len, do_create));
    return ip;
}


/* ( -- ) ( R: nest-sys -- ) runtime semantics */
vminstr_p
do_does(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    xt_ft create_def = NAME_XT(*DICT.current);

    if (create_def->handler != do_create) {
	/* XXX - this isn't *exactly* specified in the standard */
	THROW(vm, -31);
    }

    CHECK_POP(vm, 1);
    create_def[1].arg->ip = (vminstr_p) POP(vm);
    return ip;
}


/* ( c-addr - c-addr 0 | xt -1 | xt 1 ) */
vminstr_p
x_find(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    CHECK_PUSH(vm, 1);
    a_addr_ft sp = SP(vm);
    c_addr_ft counted = (c_addr_ft) PICK(sp, 0);
    SET_SP(vm, sp, -1);

    name_p nm = lookup(vm, counted + 1, *counted);
    if (nm == NULL) {
	PICK(sp, -1) = 0;
    } else {
	PICK(sp, 0) = (cell_ft) NAME_XT(nm);
	PICK(sp, -1) = NAME_IS_IMMEDIATE(nm) ? 1 : -1;
    }
    return ip;
}


/* ( -- a-addr ) name execution semantics */
vminstr_p
do_variable(vminstr_p ip, vmstate_p vm, vmarg_p var_addr)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) var_addr);
    return ip;
}


/* ( "<spaces>name" -- ) execution semantics */
vminstr_p
x_variable(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft len;
    c_addr_ft id = parse_name(&len);
    linkname(addname(vm, id, len, do_variable));
    (void) allot(vm, CELL_SIZE);
    return ip;
}


/* ( -- wid ) */
vminstr_p
x_forth_wordlist(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) &DICT.forth_wordlist);
    return ip;
}


/* ( c-addr u wid -- 0 | xt 1 | xt -1 ) */
vminstr_p
x_search_wordlist(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    c_addr_ft caddr;
    cell_ft   len;
    name_p *  wl;

    CHECK_POP(vm, 3);
    wl = (name_p *) POP(vm);
    len = (cell_ft) POP(vm);
    caddr = (c_addr_ft) POP(vm);

    if (len == 0)
	THROW(vm, -16);

    name_p nm = search_wordlist(wl, caddr, len);
    if (nm == NULL) {
	PUSH(vm, 0);
    } else {
	PUSH(vm, NAME_XT(nm));
	PUSH(vm, NAME_IS_IMMEDIATE(nm) ? 1 : -1);
    }
    return ip;
}
