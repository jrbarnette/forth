/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "forth.h"
#include "dictionary.h"
#include "cmdline.h"


/*
 * terminal.c - Standard Forth words for user terminal I/O.
 */

/*------  ------  ------  ------  ------  ------  ------  ------
 * EMIT                  6.1.1320 CORE                   38
 * ACCEPT                6.1.0695 CORE                   33
 *------  ------  ------  ------  ------  ------  ------  ------
 */


/* ( x -- ) */
PRIM_HDLR(x_emit)
{
    CHECK_POP(vm, 1);
    putc((char_ft) POP(vm), stdout);
    return ip;
}


/* ( c-addr u prompt -- flag len ) */
PRIM_HDLR(x_prompt_accept)
{
    CHECK_POP(vm, 3);

    cell_ft *sp = SP(vm);
    char *buff = (char *) PICK(sp, 2);
    cell_ft buff_len = PICK(sp, 1);
    char *prompt = (char *) PICK(sp, 0);

    cell_ft have_more = F_FALSE;
    size_t len = 0;

    if (IS_INTERACTIVE(DICT.input)) {
	rl_instream = DICT.input;
	char *line = readline(prompt);
	if (line != NULL) {
	    len = strlen(line);
	    if (len > 0) {
		if (len > buff_len) {
		    len = buff_len;
		    line[len] = '\0';
		}
		add_history(line);
		memcpy(buff, line, len);
	    }
	    free(line);
	    have_more = F_TRUE;
	}
    } else if (fgets(buff, (int) buff_len, DICT.input) != NULL) {
	len = strlen(buff);
	len -= (buff[len-1] == '\n');
	have_more = F_TRUE;
    }
    DICT.lineno -= have_more;
    PICK(sp, 2) = len;
    PICK(sp, 1) = have_more;
    SET_SP(vm, sp, 1);
    return ip;
}
