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
// POSTPONE              6.1.2033 CORE                   43
// S"                    6.1.2165 CORE                   44
// [                     6.1.2500 CORE                   48
// ]                     6.1.2540 CORE                   49
//
// C"                    6.2.0855 CORE EXT               52
//------  ------  ------  ------  ------  ------  ------  ------


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
