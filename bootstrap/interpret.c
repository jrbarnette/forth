/*
 * Copyright 2011, by J. Richard Barnette
 */

#include <stdio.h>
#include <string.h>
#include <setjmp.h>

#include "forth.h"

/*
 * interpret.c - Outer (interactive) interpreter, and related Forth
 *   words.
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
    orig->offset = dest - orig - 1;
}


cell_ft
parse(char_ft c, c_addr_ft s, cell_ft len)
{
    c_addr_ft ns = memchr(s, (char) c, len);

    if (ns != NULL) {
	DICT.to_in = ns - DICT.source.c_addr + 1;
	return (cell_ft) (ns - s);
    } else {
	DICT.to_in = DICT.source.len;
	return len;
    }
}


void
execute(vmstate_p vm, xt_ft entry_xt)
{
    vminstr_p ip;

    ip = entry_xt->handler(NULL, vm, entry_xt[1].data);

    while (ip != NULL) {
	xt_ft xtok = ip->xtok;
	ip = xtok->handler(ip + 1, vm, xtok[1].data);
    }
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


static cell_ft
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

static vminstr_p
do_skip(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    return ip + ip->offset + 1;
}

static vminstr_p
do_fskip(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft tos;

    CHECK_POP(vm, 1);
    tos = POP(vm);
    if (tos == 0) ip += vm->ip->offset;
    return ip + 1;
}


/* ( "paren"		6.1.0080 CORE, p. 26 */
/* ( “ccc<paren>” -- ) execution semantics */
static vminstr_p
x_paren(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    (void) parse(')', PARSE_AREA_PTR, PARSE_AREA_LEN);
    return ip;
}



/* >IN "to-in"		6.1.0560 CORE, p. 31 */
/* ( -- a-addr ) */
static vminstr_p
x_to_in(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, &DICT.to_in);
    return ip;
}


/* ABORT		6.1.0670 CORE, p. 32 */
/* ( i*x -- ) ( R: j*x -- ) */
static vminstr_p
x_abort(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    THROW(vm, -1);
    /* NOTREACHED */
    return NULL;
}


/* EVALUATE		6.1.1360 CORE, p. 39 */
/* ( i*x c-addr u -- j*x ) */
static vminstr_p
x_evaluate(vminstr_p ip, vmstate_p vm, addr_ft ignore)
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


/* EXECUTE		6.1.1370 CORE, p. 39 */
/* ( i*x xt -- i*j ) */
static vminstr_p
x_execute(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    xt_ft xtok;

    CHECK_POP(vm, 1);
    xtok = (xt_ft)POP(vm);
    return xtok->handler(ip, vm, xtok[1].data);
}


/* LITERAL		6.1.1780 CORE, p. 42 */
/* ( -- x ) runtime semantics */
static vminstr_p
do_literal(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, ip->cell);
    return ip + 1;
}

/* interpretation semantics undefined */
/* ( x -- ) compilation semantics */
static vminstr_p
x_literal(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    compile_literal(vm, POP(vm));
    return ip;
}


/* S" "s-quote"		6.1.2165 CORE, p. 45 */
/* ( -- c-addr u ) runtime semantics */
static vminstr_p
do_s_quote(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft len = ip->cell;

    CHECK_PUSH(vm, 2);
    PUSH(vm, ip + 1);
    PUSH(vm, len);
    return (vminstr_p) ((cell_ft) ip + ALIGNED(len + CELL_SIZE));
}


/* interpretation semantics undefined */
/* ( "ccc<quote>" -- ) compilation semantics */
static vminstr_p
x_s_quote(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    char_ft *str_src = PARSE_AREA_PTR;
    size_t len = parse('"', str_src, PARSE_AREA_LEN);
    char_ft *str_dst;

    compile_xt(vm, S_QUOTE_XT);
    COMMA(vm, len);
    str_dst = allot(vm, len);
    memcpy(str_dst, str_src, len);
    ALIGN(vm);
    return ip;
}


/* SOURCE		6.1.2216 CORE, p. 46 */
/* ( -- c-addr u ) */
static vminstr_p
x_source(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 2);
    PUSH(vm, &DICT.source.c_addr);
    PUSH(vm, DICT.source.len);
    return ip;
}


/* QUIT			6.1.2050 CORE, p. 44 */
/* ( -- )  ( R: i*x -- ) */
static vminstr_p
x_quit(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    CLEAR_RSTACK(vm);
    THROW(vm, -56);
    /* NOTREACHED */
    return NULL;
}


/* STATE		6.1.2250 CORE, p. 46 */
/* ( -- a-addr ) */
static vminstr_p
x_state(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) &DICT.state);
    return ip;
}


/* POSTPONE		6.1.2033 CORE, p. 43 */
/* ( -- ) execution semantics for default compilation semantics */
static vminstr_p
do_postpone(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    compile_xt(vm, ip->xtok);
    return ip + 1;
}

/* ( "<spaces>name" -- ) compilation semantics */
static vminstr_p
x_postpone(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    c_addr_ft	namestr = PARSE_AREA_PTR;
    size_t	len = parse(' ', namestr, PARSE_AREA_LEN);
    name_p	name = lookup(namestr, len);
    xt_ft	xtok;

    if (name == NULL) {
	THROW(vm, -58);
    }

    xtok = NAME_XT(name);
    if (NAME_IS_IMMEDIATE(name)) {
	compile_xt(vm, xtok);
    } else {
	compile_postpone(vm, xtok);
    }
    return ip;
}


/* [ "left-bracket"	6.1.2500 CORE, p. 49 */
/* interpretation semantics undefined */
/* ( -- ) compilation semantics */
static vminstr_p
x_left_bracket(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    DICT.state = STATE_INTERP;
    return ip;
}


/* ] "right-bracket"	6.1.2540 CORE, p. 50 */
/* ( -- ) */
static vminstr_p
x_right_bracket(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    DICT.state = STATE_COMPILE;
    return ip;
}


#if 0
/* PARSE		6.2.2008 CORE EXT, p. 57 */
/* ( char -- ) */
static vminstr_p
x_parse(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
}
#endif


/* \ "backslash"	6.2.2535 CORE EXT, p. 60 */
/* ( “ccc<eol>” -- ) execution semantics */
static vminstr_p
x_backslash(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    DICT.to_in = DICT.source.len;
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
}


defn_dt
interpret_defns[] = {
    { initialize_xtokens },
    { define_name, "(",		x_paren, NAME_TYPE_IMMEDIATE },
    { define_name, ">IN",	x_to_in },
    { define_name, "ABORT",	x_abort },
    { define_name, "EVALUATE",	x_evaluate },
    { define_name, "EXECUTE",	x_execute },
    { define_name, "LITERAL",	x_literal, NAME_TYPE_COMPILE },
    { define_name, "S\"",	x_s_quote, NAME_TYPE_COMPILE },
    { define_name, "POSTPONE",	x_postpone, NAME_TYPE_COMPILE },
    { define_name, "QUIT",	x_quit },
    { define_name, "SOURCE",	x_source },
    { define_name, "STATE",	x_state },
    { define_name, "[",		x_left_bracket, NAME_TYPE_COMPILE },
    { define_name, "]",		x_right_bracket },
    { define_name, "\\",	x_backslash, NAME_TYPE_IMMEDIATE },
    { NULL }
};

#if 0
    ."                    6.1.0190 CORE                   28
    ABORT"                6.1.0680 CORE                   32
    BL                    6.1.0770 CORE                   34
    CHAR                  6.1.0895 CORE                   35
    COUNT                 6.1.0980 CORE                   36
    ENVIRONMENT?          6.1.1345 CORE                   38
    WORD                  6.1.2450 CORE                   49
    [']                   6.1.2510 CORE                   50
    [CHAR]                6.1.2520 CORE                   50
    #TIB                  6.2.0060 CORE EXT               51
    C"                    6.2.0855 CORE EXT               53
    COMPILE,              6.2.0945 CORE EXT               54
    FALSE                 6.2.1485 CORE EXT               55
    REFILL                6.2.2125 CORE EXT               57
    RESTORE-INPUT         6.2.2148 CORE EXT               57
    SAVE-INPUT            6.2.2182 CORE EXT               58
    SOURCE-ID             6.2.2218 CORE EXT               58
    TIB                   6.2.2290 CORE EXT               58
    TRUE                  6.2.2298 CORE EXT               59
    [COMPILE]             6.2.2530 CORE EXT               60
#endif
