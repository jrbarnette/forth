/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

/* ISO/IEC standard */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/* IEEE 1003 (POSIX) */
#include <unistd.h>

/* 3rd party library */
#include <readline/readline.h>
#include <readline/history.h>

#include "forth.h"
#include "terminal.h"

struct terminal_config termconfig;

bool
term_open(char *filename)
{
    termconfig.input = fopen(filename, "r");
    if (termconfig.input == NULL) {
	return false;
    }
    termconfig.filename = filename;
    termconfig.lineno = 0;
    termconfig.is_interactive = false;
    return true;
}


void
term_close(void)
{
    termconfig.filename = NULL;
    fclose(termconfig.input);
}


void
term_set_input(FILE *fp, bool is_interactive)
{
    termconfig.input = fp;
    termconfig.filename = NULL;
    termconfig.lineno = 0;
    termconfig.is_interactive = is_interactive
	|| isatty(fileno(termconfig.input)) != 0;
}


/* ( x -- ) */
void
term_emit(char c)
{
    putc(c, stdout);
}


/* ( c-addr u prompt -- len flag ) */
cell_ft
term_readline(char *prompt, char *buff, size_t *buff_len)
{
    cell_ft have_more = F_FALSE;
    size_t len = 0;

    if (termconfig.is_interactive) {
	rl_instream = termconfig.input;
	char *line = readline(prompt);
	if (line != NULL) {
	    len = strlen(line);
	    if (len > 0) {
		if (len > *buff_len) {
		    len = *buff_len;
		    line[len] = '\0';
		}
		add_history(line);
		memcpy(buff, line, len);
	    }
	    free(line);
	    have_more = F_TRUE;
	    termconfig.lineno++;
	}
    } else if (fgets(buff, (int) *buff_len, termconfig.input) != NULL) {
	len = strlen(buff);
	len -= (buff[len-1] == '\n');
	have_more = F_TRUE;
	termconfig.lineno++;
    }
    *buff_len = len;
    return have_more;
}
