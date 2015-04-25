/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "forth.h"

/*
 * interpret.c - Outer (interactive) interpreter, and related Forth
 *   words.
 */

/*------  ------  ------  ------  ------  ------  ------  ------
  >IN                   6.1.0560 CORE                   31
  ABORT                 6.1.0670 CORE                   32
  BASE                  6.1.0750 CORE                   34
  CHAR                  6.1.0895 CORE                   35
  DECIMAL               6.1.1170 CORE                   36
  EVALUATE              6.1.1360 CORE                   38
  EXECUTE               6.1.1370 CORE                   38
  LITERAL               6.1.1780 CORE                   41
  POSTPONE              6.1.2033 CORE                   43
  QUIT                  6.1.2050 CORE                   43
  S"                    6.1.2165 CORE                   44
  SOURCE                6.1.2216 CORE                   45
  STATE                 6.1.2250 CORE                   45
  [                     6.1.2500 CORE                   48
  ]                     6.1.2540 CORE                   49

  C"                    6.2.0855 CORE EXT               52
  HEX                   6.2.1660 CORE EXT               54
  PARSE                 6.2.2008 CORE EXT               55
  REFILL                6.2.2125 CORE EXT               55
  ------  ------  ------  ------  ------  ------  ------  ------
*/


void
compile_literal(vmstate_p vm, cell_ft n)
{
    COMMA(vm, DO_LITERAL_XT);
    COMMA(vm, n);
}


void
compile_postpone(vmstate_p vm, xt_ft xtok)
{
    COMMA(vm, DO_POSTPONE_XT);
    COMMA(vm, xtok);
}


void
compile_xt(vmstate_p vm, xt_ft xtok)
{
    COMMA(vm, xtok);
}


vminstr_p
compile_skip(vmstate_p vm, xt_ft skip)
{
    compile_xt(vm, skip);
    return (vminstr_p) allot(vm, CELL_SIZE);
}


void
patch(vminstr_p orig, vminstr_p dest)
{
    orig->offset = dest - orig;
}


cell_ft
parse(char_ft c, c_addr_ft s, cell_ft len)
{
    if (c == ' ') {
	unsigned i;
	for (i = 0; i < len; i++) {
	    if (!isgraph(s[i])) {
		DICT.to_in = s + i - DICT.source.c_addr + 1;
		return (cell_ft) i;
	    }
	}
	DICT.to_in = DICT.source.len;
	return len;
    } else {
	c_addr_ft ns = memchr(s, (char) c, len);
	if (ns != NULL) {
	    DICT.to_in = ns - DICT.source.c_addr + 1;
	    return (cell_ft) (ns - s);
	} else {
	    DICT.to_in = DICT.source.len;
	    return len;
	}
    }
}


c_addr_ft
parse_name(cell_ft *p_len)
{
    c_addr_ft	parse_area = PARSE_AREA_PTR;
    cell_ft	parse_len = PARSE_AREA_LEN;

    while (parse_len > 0 && !isgraph(*parse_area)) {
	parse_area++;
	parse_len--;
    }
    *p_len = parse(' ', parse_area, parse_len);
    return parse_area;
}


void
execute(vmstate_p vm, xt_ft entry_xt)
{
    vminstr_p ip;

    ip = entry_xt->handler(NULL, vm, entry_xt[1].arg);

    while (ip != NULL) {
	xt_ft xtok = ip->xtok;
	ip = xtok->handler(ip + 1, vm, xtok[1].arg);
    }
}


static int
evaluate_name(c_addr_ft s, cell_ft len, vmstate_p vm)
{
    name_p name = lookup(vm, s, len);
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
	    /* accepting lower-case is non-standard behavior */
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
	PUSH(vm, n);
    } else {
	compile_literal(vm, n);
    }

    return 1;
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


static cell_ft
refill(void)
{
    char *		line;
    const char *	prompt;
    cell_ft		len;

    if (IS_INTERACTIVE(DICT.input)) {
	if (DICT.state == STATE_INTERP) {
	    prompt = "ok ";
	} else {
	    prompt = "";
	}

	rl_instream = DICT.input;
	line = readline(prompt);
	if (line == NULL) {
	    return F_FALSE;
	}
	len = strlen(line);
	if (len > DICT.source_max_len) {
	    len = DICT.source_max_len;
	    line[len] = '\0';
	}
	add_history(line);
	memcpy(DICT.source.c_addr, line, len);
    } else {
	line = (char *) DICT.source.c_addr;
	if (fgets(line, DICT.source_max_len, DICT.input) == NULL) {
	    return F_FALSE;
	}
	len = strlen(line);

	if (line[len-1] == '\n')
	    len--;
    }
    DICT.source.len = len;
    DICT.to_in = 0;
    DICT.lineno++;
    return F_TRUE;
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

static vminstr_p
do_skip(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    return ip + ip->offset;
}


static vminstr_p
do_fskip(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    if (POP(vm) == 0)
	return ip + ip->offset;
    else
	return ip + 1;
}


/* ( -- a-addr ) */
static vminstr_p
x_to_in(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, &DICT.to_in);
    return ip;
}


/* ( i*x -- ) ( R: j*x -- ) */
static vminstr_p
x_abort(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CLEAR_STACK(vm);
    CLEAR_RSTACK(vm);
    THROW(vm, -1);
    /* NOTREACHED */
    return NULL;
}


/* ( -- a-addr ) */
static vminstr_p
x_base(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) &DICT.base);
    return ip;
}


/* ( "<spaces>name" -- char ) */
static vminstr_p
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
static vminstr_p
x_decimal(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    DICT.base = 10;
    return ip;
}


/* ( i*x c-addr u -- j*x ) */
static vminstr_p
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


/* ( i*x xt -- i*j ) */
static vminstr_p
x_execute(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    xt_ft xtok;

    CHECK_POP(vm, 1);
    xtok = (xt_ft)POP(vm);
    return xtok->handler(ip, vm, xtok[1].arg);
}


/* ( -- x ) runtime semantics */
static vminstr_p
do_literal(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, ip->cell);
    return ip + 1;
}


/* ( x -- ) compilation semantics */
static vminstr_p
x_literal(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    compile_literal(vm, POP(vm));
    return ip;
}


/* ( -- c-addr u ) runtime semantics */
static vminstr_p
do_s_quote(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft len = ip->cell;

    CHECK_PUSH(vm, 2);
    PUSH(vm, ip + 1);
    PUSH(vm, len);
    return (vminstr_p) ((cell_ft) ip + XALIGNED(len + CELL_SIZE));
}


/* ( "ccc<quote>" -- ) compilation semantics */
static vminstr_p
x_s_quote(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    char_ft *str_src = PARSE_AREA_PTR;
    cell_ft len = parse('"', str_src, PARSE_AREA_LEN);
    char_ft *str_dst;

    compile_xt(vm, S_QUOTE_XT);
    COMMA(vm, len);
    str_dst = allot(vm, XALIGNED(len));
    memcpy(str_dst, str_src, len);
    return ip;
}


/* ( -- c-addr u ) */
static vminstr_p
x_source(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 2);
    PUSH(vm, DICT.source.c_addr);
    PUSH(vm, DICT.source.len);
    return ip;
}


/* ( -- )  ( R: i*x -- ) */
static vminstr_p
x_quit(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CLEAR_RSTACK(vm);
    THROW(vm, -56);
    /* NOTREACHED */
    return NULL;
}


/* ( -- a-addr ) */
static vminstr_p
x_state(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) &DICT.state);
    return ip;
}


/* ( -- ) execution semantics for default compilation semantics */
static vminstr_p
do_postpone(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    compile_xt(vm, ip->xtok);
    return ip + 1;
}


/* ( "<spaces>name" -- ) compilation semantics */
static vminstr_p
x_postpone(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
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
	compile_xt(vm, xtok);
    } else {
	compile_postpone(vm, xtok);
    }
    return ip;
}


/* ( -- ) compilation semantics */
static vminstr_p
x_left_bracket(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    DICT.state = STATE_INTERP;
    return ip;
}


/* ( -- ) */
static vminstr_p
x_right_bracket(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    DICT.state = STATE_COMPILE;
    return ip;
}


/* ( -- c-addr u ) runtime semantics */
static vminstr_p
do_c_quote(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft len = ip->cdata[0];

    CHECK_PUSH(vm, 1);
    PUSH(vm, &ip->cdata[0]);
    return (vminstr_p) ((cell_ft) ip + XALIGNED(len + 1));
}


/* ( "ccc<quote>" -- ) compilation semantics */
static vminstr_p
x_c_quote(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    char_ft *str_src = PARSE_AREA_PTR;
    cell_ft len = parse('"', str_src, PARSE_AREA_LEN);
    char_ft *str_dst;

    compile_xt(vm, C_QUOTE_XT);
    str_dst = allot(vm, XALIGNED(len + 1));
    str_dst[0] = (char_ft) len;
    memcpy(str_dst + 1, str_src, len);
    return ip;
}


/* ( -- ) */
static vminstr_p
x_hex(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    DICT.base = 16;
    return ip;
}


/* ( char "ccc<char>" -- c-addr u ) */
static vminstr_p
x_parse(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft	len;
    c_addr_ft	parse_ptr = PARSE_AREA_PTR;
    CHECK_POP(vm, 1);
    CHECK_PUSH(vm, 1);

    len = parse((char_ft) POP(vm), parse_ptr, PARSE_AREA_LEN);
    PUSH(vm, (cell_ft) parse_ptr);
    PUSH(vm, (cell_ft) len);

    return ip;
}


/* ( -- flag ) */
static vminstr_p
x_refill(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, refill());
    return ip;
}


static void
initialize_xtokens(vmstate_p vm, defn_data_p ignore)
{
    DICT.literal_instr.handler = do_literal;
    DICT.postpone_instr.handler = do_postpone;
    DICT.skip_instr.handler = do_skip;
    DICT.fskip_instr.handler = do_fskip;
    DICT.s_quote_instr.handler = do_s_quote;
    DICT.c_quote_instr.handler = do_c_quote;

    DICT.base = 10;
}


defn_dt
interpret_defns[] = {
    { initialize_xtokens },
    { define_name, ">IN",	x_to_in },
    { define_name, "ABORT",	x_abort },
    { define_name, "BASE",	x_base },
    { define_name, "CHAR",	x_char },
    { define_name, "DECIMAL",	x_decimal },
    { define_name, "EVALUATE",	x_evaluate },
    { define_name, "EXECUTE",	x_execute },
    { define_name, "LITERAL",	x_literal, NAME_TYPE_COMPILE },
    { define_name, "POSTPONE",	x_postpone, NAME_TYPE_COMPILE },
    { define_name, "QUIT",	x_quit },
    { define_name, "SOURCE",	x_source },
    { define_name, "STATE",	x_state },
    { define_name, "S\"",	x_s_quote, NAME_TYPE_COMPILE },
    { define_name, "[",		x_left_bracket, NAME_TYPE_COMPILE },
    { define_name, "]",		x_right_bracket },

    { define_name, "C\"",	x_c_quote, NAME_TYPE_COMPILE },
    { define_name, "HEX",	x_hex },
    { define_name, "PARSE",	x_parse },
    { define_name, "REFILL",	x_refill },
    { NULL }
};
