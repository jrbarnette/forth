/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "forth.h"
#include "cmdline.h"

/*
 * parse.c - C utilities and Forth primitives relating to parsing
 *   and input sources.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// PARSE                 6.2.2008 CORE EXT               55
// REFILL                6.2.2125 CORE EXT               55
//------  ------  ------  ------  ------  ------  ------  ------


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


cell_ft
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


/* -------------------------------------------------------------- */


/* ( char "ccc<char>" -- c-addr u ) */
vminstr_p
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
vminstr_p
x_refill(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft have_more;
    CHECK_PUSH(vm, 1);
    if (DICT.source_id == SOURCE_ID_TERMINAL) {
        have_more = refill();
    } else {
        have_more = F_FALSE;
    }
    PUSH(vm, have_more);
    return ip;
}
