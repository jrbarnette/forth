/*
 * Copyright 2009, by J. Richard Barnette
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


/*
 * Look up a definition in the dictionary, and return its execution
 * token.  Return NULL if not found.
 */
name_p
lookup(c_addr_ft id, size_t len)
{
    name_p	cur;

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
    assert(HERE == xtok[1].data);

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
    c_addr_ft	id = (c_addr_ft) data->data[0];
    vminstr_fn	hdlr = (vminstr_fn) data->data[1];
    cell_ft	len = strlen((char *) id);

    name_p nm = addname(vm, id, len, hdlr);
    linkname(nm);
    NAME_SET_TYPE(nm, data->flags);
}


/* -------------------------------------------------------------- */

/* ' "tick"		6.1.0070 CORE, p. 25 */
/* ( "<spaces>name" -- xt ) */
static cell_ft
x_tick(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    c_addr_ft id = PARSE_AREA_PTR;
    cell_ft len = parse(' ', id, PARSE_AREA_LEN);
    name_p nm = lookup(id, len);

    if (nm == NULL)
	THROW(vm, -13);

    CHECK_PUSH(vm, 1);
    PUSH(vm, tos);
    return (cell_ft) NAME_XT(nm);
}


/* : "colon"		6.1.0450 CORE, p. 30 */
/* ( R: -- nest-sys ) initiation semantics */
static cell_ft
do_colon(cell_ft tos, vmstate_p vm, addr_ft newip)
{
    CHECK_RPUSH(vm, 1);
    RPUSH(vm, vm->ip);
    vm->ip = (vm_instr_p) newip;
    return tos;
}

/* ( C: "<spaces> name" -- colon-sys ) */
static cell_ft
x_colon(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    c_addr_ft id = PARSE_AREA_PTR;
    cell_ft len = parse(' ', id, PARSE_AREA_LEN);
    CHECK_PUSH(vm, 1);
    PUSH(vm, tos);
    DICT.state = STATE_COMPILE;
    return (cell_ft) addname(vm, id, len, do_colon);
}


/* ; "semicolon"	6.1.0460 CORE, p. 30 */
/* ( C: colon-sys -- ) compilation semantics */
static cell_ft
x_semicolon(cell_ft tos, vmstate_p vm, addr_ft exit_xt_ptr)
{
    CHECK_POP(vm, 1);
    compile_xt(vm, *(xt_ft *) exit_xt_ptr);
    linkname((name_p) tos);
    DICT.state = STATE_INTERP;
    return POP(vm);
}


static void
define_semicolon(vmstate_p vm, defn_data_p data)
{
    name_p exit_nm = lookup((c_addr_ft) "EXIT", 4);
    define_name(vm, data);
    COMMA(vm, NAME_XT(exit_nm));
}


/* >BODY "to-body"	6.1.0550 CORE, p. 31 */
/* ( xt -- a-addr ) */
static cell_ft
x_to_body(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    return (cell_ft) ((xt_ft) tos + 2);
}


/* EXIT			6.1.1380 CORE, p. 39 */
/* interpretation semantics undefined */
/* ( R: nest-sys -- ) execution semantics */
static cell_ft
x_exit(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_RPOP(vm, 1);
    vm->ip = (vm_instr_p) RPOP(vm);
    return tos;
}


/* CONSTANT		6.1.0950 CORE, p. 35 */
/* ( -- x ) name execution semantics */
static cell_ft
do_constant(cell_ft tos, vmstate_p vm, addr_ft data_ptr)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, tos);
    return *(cell_ft *)data_ptr;
}

/* ( “<spaces>name” -- ) */
static cell_ft
x_constant(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    c_addr_ft id = PARSE_AREA_PTR;
    cell_ft len;

    CHECK_POP(vm, 1);
    len = parse(' ', id, PARSE_AREA_LEN);
    linkname(addname(vm, id, len, do_constant));
    COMMA(vm, tos);
    return POP(vm);
}



/* CREATE		6.1.1000 CORE, p. 36 */
/* ( -- a-addr ) name execution semantics */
static cell_ft
do_create(cell_ft tos, vmstate_p vm, addr_ft data_ptr)
{
    vm_instr_p does_xt = *(vm_instr_p *)data_ptr;

    CHECK_PUSH(vm, 1);
    PUSH(vm, tos);
    if (does_xt != NULL) {
	CHECK_RPUSH(vm, 1);
	RPUSH(vm, vm->ip);
	vm->ip = does_xt;
    }
    return (cell_ft) ((a_addr_ft) data_ptr + 1);
}

/* ( “<spaces>name” -- ) */
static cell_ft
x_create(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    c_addr_ft id = PARSE_AREA_PTR;
    cell_ft len = parse(' ', id, PARSE_AREA_LEN);
    linkname(addname(vm, id, len, do_create));
    COMMA(vm, (xt_ft) NULL);
    return tos;
}


/* DOES> "does"		6.1.1250 CORE, p. 37 */
/* ( -- ) ( R: nest-sys -- ) runtime semantics */
static cell_ft
do_does(cell_ft tos, vmstate_p vm, addr_ft data_ptr)
{
    xt_ft create_def = NAME_XT(DICT.namelist);

    if (create_def[0].handler != do_create) {
	/* XXX - this isn't *exactly* specified in the standard */
	THROW(vm, -31);
    }

    ((vm_instr_p *) create_def)[1] = vm->ip;
    CHECK_RPOP(vm, 1);
    vm->ip = (vm_instr_p) RPOP(vm);
    return tos;
}

/* interpretation semantics undefined */
/* ( C: colon-sys1 -- colon-sys2 ) compilation semantics */
static cell_ft
x_does(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    compile_xt(vm, DOES_XT);
    return tos;
}


/* FIND			6.1.1550 CORE, p. 39 */
/* ( c-addr -- c-addr 0 | xt 1 | xt -1 ) */
static cell_ft
x_find(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    c_addr_ft caddr = (c_addr_ft) tos;
    name_p    nm;

    CHECK_POP(vm, 1);
    CHECK_PUSH(vm, 1);

    nm = lookup(caddr + 1, (size_t) *caddr);
    if (nm == NULL) {
	PUSH(vm, tos);
	return 0;
    } else {
	PUSH(vm, NAME_XT(nm));
	return NAME_IS_IMMEDIATE(nm) ? 1 : -1;
    }
}


/* IMMEDIATE		6.1.1710 CORE, p. 41 */
/* ( -- ) */
static cell_ft
x_immediate(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    NAME_SET_TYPE(DICT.namelist, NAME_TYPE_IMMEDIATE);
    return tos;
}


/* VARIABLE		6.1.2410 CORE, p. 48 */
/* ( -- a-addr ) name execution semantics */
static cell_ft
do_variable(cell_ft tos, vmstate_p vm, addr_ft varaddr)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, tos);
    return (cell_ft) varaddr;
}

/* ( "<spaces>name" -- ) execution semantics */
static cell_ft
x_variable(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    c_addr_ft id = PARSE_AREA_PTR;
    cell_ft len = parse(' ', id, PARSE_AREA_LEN);
    linkname(addname(vm, id, len, do_variable));
    (void) allot(vm, CELL_SIZE);
    return tos;
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

    /* EXIT out of order because define_semicolon() needs it */
    { define_name, "EXIT",	x_exit, NAME_TYPE_NO_INTERPRET },
    { define_semicolon, ";",	x_semicolon, NAME_TYPE_COMPILE },

    { define_name, ">BODY",	x_to_body },
    { define_name, "CONSTANT",	x_constant },
    { define_name, "CREATE",	x_create },
    { define_name, "DOES>",	x_does, NAME_TYPE_COMPILE },
    { define_name, "FIND",	x_find },
    { define_name, "IMMEDIATE",	x_immediate },
    { define_name, "VARIABLE",	x_variable },
    { NULL }
};

#if 0
    :NONAME               6.2.0455 CORE EXT               52
    MARKER                6.2.1850 CORE EXT               56
    TO                    6.2.2295 CORE EXT               59
    VALUE                 6.2.2405 CORE EXT               60
#endif
