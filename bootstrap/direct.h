/*
 * Copyright 2017, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef DIRECT_H
#define DIRECT_H

#include "forth.h"

/*
 * Internal support for writing direct-threaded code and for implementing
 * the primitives called from the direct-thread interpreter loop.
 */

/* direct threaded name definition primitives */
extern PRIM_HDLR(i_startname);
extern PRIM_HDLR(i_addname);
extern PRIM_HDLR(i_setflags);
extern PRIM_HDLR(i_linkname);

/* meta interpretation primitives */
extern PRIM_HDLR(meta_interpret);
extern PRIM_HDLR(meta_compile);
extern PRIM_HDLR(i_lookup);


#define X(x)		{ .handler = x },
#define S(s)		{ .id = s },
#define N(n)		{ .cell = (cell_ft) (n) },
#define L(x)		X(do_literal) N(x)

#define DIRECT_FORTH(nm)	vminstr_ft nm[] = {
#define END_DIRECT		X(x_exit) };


/*
 * Miscellaneous C utility declarations.
 */

extern addr_ft allot(vmstate_ft *, cell_ft);

#define COMMA(vm, x)	(*(a_addr_ft)allot((vm), CELL_SIZE) = (cell_ft) (x))
#define COMPILE(vm, xt)	COMMA(vm, xt)

#endif // DIRECT_H
