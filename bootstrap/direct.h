/*
 * Copyright 2017, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef DIRECT_H
#define DIRECT_H

#include "forth.h"

/*
 * Support for dictionary initialization by interpreting direct-threaded
 * code stored in arrays.
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

#endif // DIRECT_H
