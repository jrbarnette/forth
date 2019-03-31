/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "forth.h"

/*
 * interpret.c - Outer (interactive) interpreter, and related Forth
 *   words.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// ABORT                 6.1.0670 CORE                   32
// CHAR                  6.1.0895 CORE                   35
// DECIMAL               6.1.1170 CORE                   36
// EVALUATE              6.1.1360 CORE                   38
// POSTPONE              6.1.2033 CORE                   43
// QUIT                  6.1.2050 CORE                   43
// S"                    6.1.2165 CORE                   44
// [                     6.1.2500 CORE                   48
// ]                     6.1.2540 CORE                   49
//
// C"                    6.2.0855 CORE EXT               52
// HEX                   6.2.1660 CORE EXT               54
//------  ------  ------  ------  ------  ------  ------  ------


static bool
evaluate_name(c_addr_ft s, cell_ft len, vmstate_p vm)
{
    name_p name = lookup(vm, s, len);
    xt_ft xtok;

    if (name == NULL) {
	return false;
    }

    xtok = NAME_XT(name);
    if (DICT.state != STATE_INTERP && !NAME_IS_IMMEDIATE(name)) {
	COMPILE(vm, xtok);
    } else if (DICT.state != STATE_INTERP || NAME_IS_INTERPRETABLE(name)) {
	execute(vm, xtok);
    } else {
	THROW(vm, -14);
    }

    return true;
}


static void
execute_literal(vmstate_p vm, cell_ft n)
{
    if (DICT.state == STATE_INTERP) {
	CHECK_PUSH(vm, 1);
	PUSH(vm, n);
    } else {
	COMPILE(vm, DO_LITERAL_XT);
	COMMA(vm, n);
    }
}


static bool
evaluate_number(c_addr_ft s, cell_ft len, vmstate_p vm)
{
    cell_ft	base = DICT.base;
    int		sign = 1;
    unsigned	i = 0;
    cell_ft	n = 0;

    if (len == 0 || base > 'Z' - 'A' + 11)
	return false;

    if (s[0] == '-') {
	i++;
	sign = -1;
	if (len == 1)
	    return false;
    }

    for (; i < len; i++) {
	char_ft	dchar = s[i];
	int	dig;

	if ('0' <= dchar && dchar <= '9') {
	    dig = dchar - '0';
	} else if ('A' <= dchar && dchar <= 'Z') {
	    dig = dchar - 'A' + 10;
	} else if ('a' <= dchar && dchar <= 'z') {
	    /* accepting lower-case is non-standard behavior */
	    dig = dchar - 'a' + 10;
	} else {
	    return false;
	}

	if (dig >= base)
	    return false;

	/* XXX check for overflow - not specified by standard */
	n = base * n + dig;
    }

    if (sign < 0)
	n = -n;

    execute_literal(vm, n);
    return true;
}


/* interpreter for QUIT and EVALUATE */
static void
evaluate(vmstate_p vm)
{
    while (PARSE_AREA_LEN != 0) {
	cell_ft len;
	c_addr_ft id = parse_name(&len);

	if (len == 0)
	    break;

	if (!evaluate_name(id, len, vm) &&
		!evaluate_number(id, len, vm)) {
	    fwrite(id, sizeof (*id), len, stderr);
	    fputc(' ', stderr);
	    THROW(vm, -13);
	}
    }
}


void
quit(vmstate_p vm, FILE *input)
{
    DICT.source_id = SOURCE_ID_TERMINAL;
    DICT.source.c_addr = DICT.tib;
    DICT.source_max_len = sizeof (DICT.tib);
    DICT.lineno = 0;
    DICT.input = input;

    DICT.state = STATE_INTERP;

    while (refill()) {
	evaluate(vm);
    }
}


void
interpret_string(vmstate_p vm, char *s)
{
    char       *eval_start;
    char       *eval_end;
    size_t	eval_remaining;

    DICT.source_id = SOURCE_ID_EVALUATE;
    DICT.state = STATE_INTERP;

    eval_start = s;
    eval_remaining = strlen(s);
    while (eval_remaining != 0) {
	eval_end = strchr(eval_start, '\n');
	if (eval_end != NULL) {
	    DICT.source.len = eval_end - eval_start;
	    eval_remaining -= DICT.source.len + 1;
	} else {
	    DICT.source.len = eval_remaining;
	    eval_remaining = 0;
	}
	DICT.source.c_addr = (c_addr_ft) eval_start;
	DICT.to_in = 0;
	evaluate(vm);
	eval_start += DICT.source.len + 1;
    }

    assert(DICT.state == STATE_INTERP);
    /* assert stacks are empty */
}


/* -------------------------------------------------------------- */

/* ( i*x -- ) ( R: j*x -- ) */
vminstr_p
x_abort(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CLEAR_STACK(vm);
    CLEAR_RSTACK(vm);
    THROW(vm, -1);
    /* NOTREACHED */
    return NULL;
}


/* ( "<spaces>name" -- char ) */
vminstr_p
x_char(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft len;
    c_addr_ft id = parse_name(&len);

    if (len == 0)
	THROW(vm, -16);

    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) *id);
    return ip;
}


/* ( -- ) */
vminstr_p
x_decimal(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    DICT.base = 10;
    return ip;
}


/* ( i*x c-addr u -- j*x ) */
vminstr_p
x_evaluate(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
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

    evaluate(vm);

    DICT.source_id = osource_id;
    DICT.source.len = osource_len;
    DICT.source.c_addr = osource_addr;
    DICT.to_in = oto_in;

    return ip;
}


/* ( -- x ) runtime semantics */
vminstr_p
do_literal(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, ip->cell);
    return ip + 1;
}


/* ( -- c-addr u ) runtime semantics */
vminstr_p
do_s_quote(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft len = ip->cell;

    CHECK_PUSH(vm, 2);
    PUSH(vm, ip + 1);
    PUSH(vm, len);
    return (vminstr_p) ((cell_ft) ip + XALIGNED(len + CELL_SIZE));
}


/* ( "ccc<quote>" -- ) compilation semantics */
vminstr_p
x_s_quote(vminstr_p ip, vmstate_p vm, vmarg_p s_quote_xt)
{
    char_ft *str_src = PARSE_AREA_PTR;
    cell_ft len = parse('"', str_src, PARSE_AREA_LEN);
    char_ft *str_dst;

    COMPILE(vm, s_quote_xt);
    COMMA(vm, len);
    str_dst = allot(vm, XALIGNED(len));
    memcpy(str_dst, str_src, len);
    return ip;
}


/* ( -- )  ( R: i*x -- ) */
vminstr_p
x_quit(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CLEAR_RSTACK(vm);
    THROW(vm, -56);
    /* NOTREACHED */
    return NULL;
}


/* ( -- ) execution semantics for default compilation semantics */
vminstr_p
do_postpone(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    COMPILE(vm, ip->xtok);
    return ip + 1;
}


/* ( "<spaces>name" -- ) compilation semantics */
vminstr_p
x_postpone(vminstr_p ip, vmstate_p vm, vmarg_p do_postpone_xt)
{
    cell_ft	len;
    c_addr_ft	id = parse_name(&len);
    name_p	name = lookup(vm, id, len);
    xt_ft	xtok;

    if (name == NULL) {
	fputs("POSTPONE ", stderr);
	fwrite(id, sizeof (*id), len, stderr);
	fputc(' ', stderr);
	THROW(vm, -13);
    }

    xtok = NAME_XT(name);
    if (NAME_IS_IMMEDIATE(name)) {
	COMPILE(vm, xtok);
    } else {
	COMPILE(vm, do_postpone_xt);
	COMMA(vm, xtok);
    }
    return ip;
}


/* ( -- ) compilation semantics */
vminstr_p
x_left_bracket(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    DICT.state = STATE_INTERP;
    return ip;
}


/* ( -- ) */
vminstr_p
x_right_bracket(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    DICT.state = STATE_COMPILE;
    return ip;
}


/* ( -- c-addr ) runtime semantics */
vminstr_p
do_c_quote(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft len = ip->cdata[0];

    CHECK_PUSH(vm, 1);
    PUSH(vm, &ip->cdata[0]);
    return (vminstr_p) ((cell_ft) ip + XALIGNED(len + 1));
}


/* ( "ccc<quote>" -- ) compilation semantics */
vminstr_p
x_c_quote(vminstr_p ip, vmstate_p vm, vmarg_p c_quote_xt)
{
    char_ft *str_src = PARSE_AREA_PTR;
    cell_ft len = parse('"', str_src, PARSE_AREA_LEN);
    char_ft *str_dst;

    COMPILE(vm, c_quote_xt);
    str_dst = allot(vm, XALIGNED(len + 1));
    str_dst[0] = (char_ft) len;
    memcpy(str_dst + 1, str_src, len);
    return ip;
}


/* ( -- ) */
vminstr_p
x_hex(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    DICT.base = 16;
    return ip;
}
