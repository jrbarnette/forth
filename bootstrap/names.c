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
  ------  ------  ------  ------  ------  ------  ------  ------
*/


/*
 * Look up a definition in the dictionary, and return its execution
 * token.  Return NULL if not found.
 */
name_p
lookup(vmstate_p vm, c_addr_ft id, cell_ft len)
{
    name_p	cur;

    if (len == 0)
	THROW(vm, -16);

    for (cur = DICT.namelist; cur != NULL; cur = cur->prev) {
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
    name->prev = DICT.namelist;
    DICT.namelist = name;
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
    xt_ft create_def = NAME_XT(DICT.namelist);

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


/* ( -- ) */
static vminstr_p
x_immediate(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    NAME_SET_TYPE(DICT.namelist, NAME_TYPE_IMMEDIATE);
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


static void
initialize_does(vmstate_p vm, defn_data_p ignore)
{
    DICT.does_instr.handler = do_does;
}


defn_dt
names_defns[] = {
    { initialize_does },
    { define_name, "'",         x_tick },
    { define_name, ":",		x_colon },

    /* EXIT out of order for reference below */
    { define_name, "EXIT",	x_exit, NAME_TYPE_NO_INTERPRET },

    { define_name, ";",		x_semicolon, NAME_TYPE_COMPILE },
    { compile_name, "EXIT" },

    { define_name, ">BODY",	x_to_body },
    { define_name, "CONSTANT",	x_constant },
    { define_name, "CREATE",	x_create },
    { define_name, "DOES>",	x_does, NAME_TYPE_COMPILE },
    { define_name, "IMMEDIATE",	x_immediate },
    { define_name, "VARIABLE",	x_variable },
    { NULL }
};
