/*
 * Copyright 2024, by J. Richard Barnette. All Rights Reserved.
 */
include(`c_defs.m4')dnl

#ifndef CFORTH_H
#define CFORTH_H

/*
 * C interface to invoke the Forth interpreter.  The interface is
 * independent of the implementation details of the underlying Forth VM.
 */

#include "ftypes.h"

#define PRIM_HDLR(hdlr)		void hdlr(void)

typedef PRIM_HDLR((*forth_handler_fn));

`#define STACK_SIZE'	STACK_SIZE
`#define RSTACK_SIZE'	RSTACK_SIZE

`#define FARGS_LEN'	FARGS_LEN

struct fargs {
    cell_ft depth;
    cell_ft stack[`FARGS_LEN'];
};

typedef union code_cell *xt_ft;

typedef union code_cell {
    cell_ft		value;
    xt_ft		xt;
    forth_handler_fn	handler;
} code_body_ft;

extern int forth_execute(struct fargs *args, xt_ft xt);

#endif // CFORTH_H
