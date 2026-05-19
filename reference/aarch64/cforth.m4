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

typedef PRIM_HDLR((*vmhdlr_fn));

typedef union code_cell		vmcode_ft;
typedef union code_cell *	vmcodeptr_ft;

union code_cell {
    vmhdlr_fn		handler;
    cell_ft		cell;
    vmcodeptr_ft	ip;
    char *		id;
};

`#define STACK_SIZE'	STACK_SIZE
`#define RSTACK_SIZE'	RSTACK_SIZE

`#define FARGS_LEN'	FARGS_LEN

struct fargs {
    cell_ft		depth;
    cell_ft		stack[`FARGS_LEN'];
};

#define FCELL1(x0)			(cell_ft)(x0)
#define FCELL2(x0, x1)			FCELL1(x0), FCELL1(x1)
#define FCELL3(x0, x1, x2)		FCELL2(x0, x1), FCELL1(x2)
#define FCELL4(x0, x1, x2, x3)		FCELL3(x0, x1, x2), FCELL1(x3)
#define FCELL5(x0, x1, x2, x3, x4)	FCELL4(x0, x1, x2, x3), FCELL1(x4)

#define FS0				{ 0 }
#define FS1(x0)				{ 1, FCELL1(x0) }
#define FS2(x0, x1)			{ 2, FCELL2(x0, x1) }
#define FS3(x0, x1, x2)			{ 3, FCELL3(x0, x1, x2) }
#define FS4(x0, x1, x2, x3)		{ 4, FCELL4(x0, x1, x2, x3) }
#define FS5(x0, x1, x2, x3, x4)		{ 5, FCELL5(x0, x1, x2, x3, x4) }

#define FARGS(a)			struct fargs a
#define FARGS0(a)			FARGS(a) = FS0
#define FARGS1(a, x0)			FARGS(a) = FS1(x0)
#define FARGS2(a, x0, x1)		FARGS(a) = FS2(x0, x1)
#define FARGS3(a, x0, x1, x2)		FARGS(a) = FS3(x0, x1, x2)
#define FARGS4(a, x0, x1, x2, x3)	FARGS(a) = FS4(x0, x1, x2, x3)
#define FARGS5(a, x0, x1, x2, x3, x4)	FARGS(a) = FS5(x0, x1, x2, x3, x4)

extern int forth_execute(vmcodeptr_ft, struct fargs *);

#endif // CFORTH_H
