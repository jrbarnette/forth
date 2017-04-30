/*
 * Copyright 2017, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef CMDLINE_H
#define CMDLINE_H

#include <stdbool.h>
#include <stdio.h>


struct options {
    bool	is_interactive;
    char *	startup_file;
    int		argc;
    char **	argv;
};

extern struct options forth_options;

#define IS_INTERACTIVE(input)	(is_interactive((input), &forth_options))

extern void process_args(int, char *[], struct options *);
extern bool is_interactive(FILE *, struct options *);

#endif
