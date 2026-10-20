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
    char *	filename;
    FILE *	input;
};

extern struct terminal_config termconfig;

extern bool term_open(char *filename);
extern void term_close(void);
extern void term_set_input(FILE *fp, bool is_interactive);

extern void term_emit(char c);
extern cell_ft term_readline(char *prompt, char *buff,
			     size_t *buff_len);

#endif // TERMINAL_H
