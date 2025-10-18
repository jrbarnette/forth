/*
 * Copyright 2024, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef CFORTH_H
#define CFORTH_H

/*
 * C interface to invoke the Forth interpreter.  The interface is
 * independent of the implementation details of the underlying Forth VM.
 */

#include "ftypes.h"
#include "xtoken.h"

typedef union code_cell vmcode_ft;
typedef union code_cell *vmcodeptr_ft;

union code_cell {
    vmhdlr_fn		handler;
    cell_ft		cell;
    vmcodeptr_ft	ip;
    char *		id;
};

#define FARGS_LEN	7

struct fargs {
    cell_ft depth;
    cell_ft stack[FARGS_LEN];
};

#define FARGS(a)		struct fargs a
#define FARG1(x0)		(cell_ft)(x0)
#define FARG2(x0, x1)		FARG1(x0), FARG1(x1)
#define FARG3(x0, x1, x2)	FARG2(x0, x1), FARG1(x2)
#define FARGS0(a)		FARGS(a) = { 0 }
#define FARGS1(a, x0)		FARGS(a) = { 1, { FARG1(x0) }}
#define FARGS2(a, x0, x1)	FARGS(a) = { 2, { FARG2(x0, x1) }}
#define FARGS3(a, x0, x1, x2)	FARGS(a) = { 3, { FARG3(x0, x1, x2) }}

extern int forth_execute(vmcodeptr_ft, struct fargs *);

extern vmcode_ft initialize_forth[];

#endif // CFORTH_H
