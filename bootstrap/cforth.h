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

typedef union code_cell *vmcodeptr_ft;

typedef union code_cell {
    vmhdlr_fn		handler;
    cell_ft		cell;
    vmcodeptr_ft	ip;
    char *		id;
} vmcode_ft;

#endif // CFORTH_H
