/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "forth.h"

/*
 * names.c - C internal functions and Forth standard words for
 *   dictionary name space.
 */

/*------  ------  ------  ------  ------  ------  ------  ------
  '                     6.1.0070 CORE                   25
  :                     6.1.0450 CORE                   30
  EXIT                  6.1.1380 CORE                   38
  ;                     6.1.0460 CORE                   30
  >BODY                 6.1.0550 CORE                   31
  CONSTANT              6.1.0950 CORE                   35
  CREATE                6.1.1000 CORE                   36
  DOES>                 6.1.1250 CORE                   37
  IMMEDIATE             6.1.1710 CORE                   40
  VARIABLE              6.1.2410 CORE                   47

  FORTH-WORDLIST     16.6.1.1595 SEARCH                119
  SEARCH-WORDLIST    16.6.1.2192 SEARCH                120
  ------  ------  ------  ------  ------  ------  ------  ------
*/


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
name_p
addname(vmstate_p vm, c_addr_ft id, cell_ft len, vminstr_fn hdlr)
{
    name_p	cur;
    xt_ft	xtok;

    if (len == 0)		THROW(vm, -16);
    if (len > NAME_MAX_LENGTH)	THROW(vm, -19);

    ALIGN(vm);
    cur = (name_p) allot(vm, NAME_SIZE(len) + CELL_SIZE);
    cur->prev = NULL;
    cur->flags = len;
    (void) memcpy(cur->ident, id, len);
    xtok = NAME_XT(cur);
    xtok->handler = hdlr;
    assert(HERE == xtok[1].arg->data);

    return cur;
}


void
linkname(name_p name)
{
    name->prev = *DICT.current;
    *DICT.current = name;
}


void
define_name(vmstate_p vm, defn_data_p data)
{
    c_addr_ft	id = (c_addr_ft) data->data0;
    vminstr_fn	hdlr = (vminstr_fn) data->data1;
    cell_ft	len = (cell_ft) strlen((char *) id);

    name_p nm = addname(vm, id, len, hdlr);
    linkname(nm);
    NAME_SET_TYPE(nm, data->flags);
}


void
compile_name(vmstate_p vm, defn_data_p data)
{
    c_addr_ft	id = (c_addr_ft) data->data0;
    cell_ft	len = (cell_ft) strlen((char *) id);
    name_p	nm = lookup(vm, id, len);

    COMMA(vm, NAME_XT(nm));
}


/* -------------------------------------------------------------- */

static vminstr_p do_create(vminstr_p, vmstate_p, vmarg_p);

/* ( "<spaces>name" -- xt ) */
static vminstr_p
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
static vminstr_p
do_colon(vminstr_p ip, vmstate_p vm, vmarg_p newip)
{
    CHECK_RPUSH(vm, 1);
    RPUSH(vm, (cell_ft)ip);
    return newip->vminstrs;
}


/* ( C: "<spaces>name" -- colon-sys ) */
static vminstr_p
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
static vminstr_p
x_semicolon(vminstr_p ip, vmstate_p vm, vmarg_p exit_xt_ptr)
{
    CHECK_POP(vm, 1);
    compile_xt(vm, exit_xt_ptr->xtok);
    linkname((name_p) POP(vm));
    DICT.state = STATE_INTERP;
    return ip;
}


/* ( xt -- a-addr ) */
static vminstr_p
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
static vminstr_p
x_exit(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_RPOP(vm, 1);
    return (vminstr_p)RPOP(vm);
}


/* ( -- x ) name execution semantics */
static vminstr_p
do_constant(vminstr_p ip, vmstate_p vm, vmarg_p data_ptr)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, data_ptr->cell);
    return ip;
}


/* ( “<spaces>name” -- ) */
static vminstr_p
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
static vminstr_p
do_create(vminstr_p ip, vmstate_p vm, vmarg_p data_ptr)
{
    vminstr_p does_ptr = data_ptr[0].ip;
    addr_ft body = data_ptr[1].data;

    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) body);
    if (does_ptr != NULL) {
	CHECK_RPUSH(vm, 1);
	RPUSH(vm, ip);
	ip = does_ptr;
    }
    return ip;
}


/* ( “<spaces>name” -- ) */
static vminstr_p
x_create(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft len;
    c_addr_ft id = parse_name(&len);
    linkname(addname(vm, id, len, do_create));
    COMMA(vm, (xt_ft) NULL);
    return ip;
}


/* ( -- ) ( R: nest-sys -- ) runtime semantics */
static vminstr_p
do_does(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    xt_ft create_def = NAME_XT(DICT.forth_wordlist);

    if (create_def->handler != do_create) {
	/* XXX - this isn't *exactly* specified in the standard */
	THROW(vm, -31);
    }

    create_def[1].arg->ip = ip;
    CHECK_RPOP(vm, 1);
    return (vminstr_p) RPOP(vm);
}


/* ( C: colon-sys1 -- colon-sys2 ) compilation semantics */
static vminstr_p
x_does(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    compile_xt(vm, DOES_XT);
    return ip;
}


/* ( c-addr - c-addr 0 | xt -1 | xt 1 ) */
static vminstr_p
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


/* ( -- ) */
static vminstr_p
x_immediate(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    NAME_SET_TYPE(DICT.forth_wordlist, NAME_TYPE_IMMEDIATE);
    return ip;
}


/* ( -- a-addr ) name execution semantics */
static vminstr_p
do_variable(vminstr_p ip, vmstate_p vm, vmarg_p var_addr)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) var_addr);
    return ip;
}


/* ( "<spaces>name" -- ) execution semantics */
static vminstr_p
x_variable(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft len;
    c_addr_ft id = parse_name(&len);
    linkname(addname(vm, id, len, do_variable));
    (void) allot(vm, CELL_SIZE);
    return ip;
}


/* ( -- wid ) */
static vminstr_p
x_forth_wordlist(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) &DICT.forth_wordlist);
    return ip;
}


/* ( c-addr u wid -- 0 | xt 1 | xt -1 ) */
static vminstr_p
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


static void
initialize_does(vmstate_p vm, defn_data_p ignore)
{
    DICT.does_instr.handler = do_does;
}


defn_dt
names_defns[] = {
    { initialize_does },
    { define_name, "'",			x_tick },
    { define_name, ":",			x_colon },

    /* EXIT out of order for reference below */
    { define_name, "EXIT",		x_exit, NAME_TYPE_NO_INTERPRET },

    { define_name, ";",			x_semicolon, NAME_TYPE_COMPILE },
    { compile_name, "EXIT" },

    { define_name, ">BODY",		x_to_body },
    { define_name, "CONSTANT",		x_constant },
    { define_name, "CREATE",		x_create },
    { define_name, "DOES>",		x_does, NAME_TYPE_COMPILE },
    { define_name, "IMMEDIATE",		x_immediate },
    { define_name, "VARIABLE",		x_variable },

    { define_name, "FORTH-WORDLIST",	x_forth_wordlist },
    { define_name, "SEARCH-WORDLIST",	x_search_wordlist },
    { define_name, "FIND",		x_find },

    { NULL }
};
