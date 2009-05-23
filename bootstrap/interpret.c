/*
 * Copyright 2009, by J. Richard Barnette
 */

#include <stdio.h>
#include <string.h>
#include <setjmp.h>

#include "forth.h"

/*
 * interpret.c - outer (interactive) interpreter
 */

void
execute(vmstate_p vm, xt_ft entry_xt)
{
    cell_ft tos;

    tos = entry_xt->handler(SAVEDTOS(vm), vm, entry_xt[1].data);

    while (vm->ip != NULL) {
	xt_ft xtok = *vm->ip++;
	tos = xtok->handler(tos, vm, xtok[1].data);
    }

    SAVEDTOS(vm) = tos;
}


void
compile_literal(vmstate_p vm, cell_ft n)
{
    COMMA(vm, DO_LITERAL_XT);
    COMMA(vm, n);
}


void
compile_xt(vmstate_p vm, xt_ft xtok)
{
    COMMA(vm, xtok);
}


cell_ft
parse(char_ft c, c_addr_ft s, cell_ft len)
{
    c_addr_ft ns = memchr(s, (char) c, len);
    if (ns != NULL) {
	len = (cell_ft) (ns - s);
	DICT.to_in += len + 1;
    } else {
	DICT.to_in += len;
    }
    return len;
}


static int
evaluate_name(c_addr_ft s, cell_ft len, vmstate_p vm)
{
    name_p name = lookup(s, len);
    xt_ft xtok;

    if (name == NULL) {
	return 0;
    }

    xtok = NAME_XT(name);
    if (DICT.state != STATE_INTERP && !NAME_IS_IMMEDIATE(name)) {
	compile_xt(vm, xtok);
    } else if (DICT.state != STATE_INTERP || NAME_IS_INTERPRETABLE(name)) {
	execute(vm, xtok);
    } else {
	THROW(vm, -14);
    }

    return 1;
}


static int
evaluate_number(c_addr_ft s, cell_ft len, vmstate_p vm)
{
    cell_ft	base = DICT.base;
    int		sign = 1;
    unsigned	i = 0;
    cell_ft	n = 0;
    
    if (len == 0 || base > 'Z' - 'A' + 11)
	return 0;

    if (s[0] == '-') {
	i++;
	sign = -1;
	if (len == 1)
	    return 0;
    }

    for (; i < len; i++) {
	char_ft	dchar = s[i];
	int	dig;

	if ('0' <= dchar && dchar <= '9') {
	    dig = dchar - '0';
	} else if ('A' <= dchar && dchar <= 'Z') {
	    dig = dchar - 'A' + 10;
	} else if ('a' <= dchar && dchar <= 'z') {
	    /* this is non-standard behavior */
	    dig = dchar - 'a' + 10;
	} else {
	    return 0;
	}

	if (dig >= base)
	    return 0;

	/* XXX check for overflow - not specified by standard */
	n = base * n + dig;
    }

    if (sign < 0)
	n = -n;

    if (DICT.state == STATE_INTERP) {
	CHECK_PUSH(vm, 1);
	PUSH(vm, SAVEDTOS(vm));
	SAVEDTOS(vm) = n;
    } else {
	compile_literal(vm, n);
    }

    return 1;
}


/* interpreter for QUIT and EVALUATE */
static void
evaluate(vmstate_p vm)
{
    c_addr_ft	parse_area = PARSE_AREA_PTR;
    cell_ft	parse_len = PARSE_AREA_LEN;

    while (parse_len != 0) {
	size_t len;

	if (*parse_area == ' ') {
	    parse_area++;
	    parse_len--;
	    continue;
	}

	len = parse(' ', parse_area, parse_len);
	if (!evaluate_name(parse_area, len, vm) &&
		!evaluate_number(parse_area, len, vm)) {
	    THROW(vm, -13);
	}
	parse_area = PARSE_AREA_PTR;
	parse_len = PARSE_AREA_LEN;
    }
}


static int
refill(void)
{
    char *	s;
    cell_ft	len;

    if (DICT.state == STATE_INTERP) {
	(void) fputs("ok ", stdout);
    }

    s = (char *) DICT.source.c_addr;
    if (fgets(s, DICT.source_max_len, stdin) == NULL) {
	return F_FALSE;
    }

    len = strlen(s);
    if (s[len-1] == '\n')
	len--;
    DICT.source.len = len;
    DICT.to_in = 0;
    return F_TRUE;
}


void
quit(vmstate_p vm)
{
    DICT.source_id = SOURCE_ID_TERMINAL;
    DICT.source.c_addr = DICT.tib;
    DICT.source_max_len = sizeof (DICT.tib);

    DICT.state = STATE_INTERP;

    while (refill()) {
	evaluate(vm);
    }
}


/* -------------------------------------------------------------- */

/* >IN "to-in"		6.1.0560 CORE, p. 31 8/
/* ( -- a-addr ) */
static cell_ft
x_to_in(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, tos);
    return (cell_ft) &DICT.to_in;
}


/* ABORT		6.1.0670 CORE, p. 32 */
/* ( i*x -- ) ( R: j*x -- ) */
static cell_ft
x_abort(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    THROW(vm, -1);
    /* NOTREACHED */
    return 0;
}


/* EVALUATE		6.1.1360 CORE, p. 39 */
/* ( i*x c-addr u -- j*x ) */
static cell_ft
x_evaluate(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    cell_ft	osource_id = DICT.source_id;
    c_addr_ft	osource_addr = DICT.source.c_addr;
    cell_ft	osource_len = DICT.source.len;
    cell_ft	oto_in = DICT.to_in;

    CHECK_POP(vm, 2);

    DICT.source_id = SOURCE_ID_EVALUATE;
    DICT.source.len = POP(vm);
    DICT.source.c_addr = (c_addr_ft) POP(vm);
    DICT.to_in = 0;

    SAVEDTOS(vm) = tos;
    evaluate(vm);
    tos = SAVEDTOS(vm);

    DICT.source_id = osource_id;
    DICT.source.len = osource_len;
    DICT.source.c_addr = osource_addr;
    DICT.to_in = oto_in;

    return tos;
}


/* EXECUTE		6.1.1370 CORE, p. 39 */
/* ( i*x xt -- i*j ) */
static cell_ft
x_execute(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    xt_ft xtok = (xt_ft) tos;

    CHECK_POP(vm, 1);
    return xtok->handler(POP(vm), vm, xtok[1].data);
}


/* LITERAL		6.1.1780 CORE, p. 42 */
/* interpration semantics undefined */
/* ( x -- ) compilation semantics */
static cell_ft
x_literal(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    compile_literal(vm, tos);
    return POP(vm);
}

/* ( -- x ) runtime semantics */
cell_ft
do_literal(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, tos);
    return (*vm->ip++)->cell;
}


/* SOURCE		6.1.2216 CORE, p. 46 */
/* ( -- c-addr u ) */
static cell_ft
x_source(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 2);
    PUSH(vm, tos);
    PUSH(vm, &DICT.source.c_addr);
    return DICT.source.len;
}


/* QUIT			6.1.2050 CORE, p. 44 */
/* ( -- )  ( R: i*x -- ) */
static cell_ft
x_quit(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CLEAR_RSTACK(vm);
    SAVEDTOS(vm) = tos;
    THROW(vm, -56);
    /* NOTREACHED */
    return 0;
}


/* STATE		6.1.2250 CORE, p. 46 */
/* ( -- a-addr ) */
static cell_ft
x_state(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, tos);
    return (cell_ft) &DICT.state;
}


/* PARSE		6.2.2008 CORE EXT, p. 57 */
/* ( char -- ) */
static cell_ft
x_parse(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
}


defn_dt
interpret_defns[] = {
    { define_name, ">IN",	x_to_in },
    { define_name, "ABORT",	x_abort },
    { define_name, "EVALUATE",	x_evaluate },
    { define_name, "EXECUTE",	x_execute },
    { define_name, "LITERAL",	x_literal, (void *) NAME_TYPE_COMPILE },
    { define_name, "QUIT",	x_quit },
    { define_name, "SOURCE",	x_source },
    { define_name, "STATE",	x_state },
    { NULL }
};

#if 0
    ABORT"                6.1.0680 CORE                   32
    WORD                  6.1.2450 CORE                   49
    #TIB                  6.2.0060 CORE EXT               51
    REFILL                6.2.2125 CORE EXT               57
    RESTORE-INPUT         6.2.2148 CORE EXT               57
    SAVE-INPUT            6.2.2182 CORE EXT               58
    SOURCE-ID             6.2.2218 CORE EXT               58
    TIB                   6.2.2290 CORE EXT               58
#endif
