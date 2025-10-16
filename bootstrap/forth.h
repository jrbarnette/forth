/*
 * Copyright 2015, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef FORTH_H
#define FORTH_H

/*
 * The headers below aren't meant to be included independently:
 * none of them include other headers that they might depend on.
 * In the list below, every header depends on the headers that
 * precede, except that "prim.h" doesn't depend on "vmcheck.h".
 *
 * All of the content included here is needed by virtually every
 * XT primitive handler, so the content isn't really independently
 * useful.  The pieces are split apart mostly to keep any one file
 * from being too big.
 */

#include "ftypes.h"
#include "vmstate.h"
#include "xtoken.h"
#include "vmcheck.h"
#include "prim.h"
#include "cforth.h"

#define FILE_MODE_R_O	((cell_ft) 01)
#define FILE_MODE_R_W	((cell_ft) 02)
#define FILE_MODE_W_O	((cell_ft) 03)
#define FILE_MODE_BIN	((cell_ft) 04)

extern void execute(vmstate_ft *, xt_ft);

extern void report_exception(int, vmstate_ft *, char *);

extern vmcode_ft initialize_forth[];

#endif // FORTH_H
