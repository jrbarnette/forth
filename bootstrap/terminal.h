/*
 * Copyright 2025, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>


struct terminal_config {
    bool	is_interactive;
    size_t	lineno;
    FILE *	input;
};

struct terminal_config termconfig;

#define IS_INTERACTIVE(input)	(term_is_interactive((input)))

extern bool term_is_interactive(FILE *fp);
extern void term_emit(char c);
extern cell_ft term_readline(char *prompt, char *buff,
			     size_t *buff_len);

#endif // TERMINAL_H
