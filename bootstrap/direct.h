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
extern vminstr_p i_startname(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p i_addname(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p i_setflags(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p i_linkname(vminstr_p, vmstate_p, vmarg_p);

/* meta interpretation primitives */
extern vminstr_p meta_interpret(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p meta_compile(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p i_lookup(vminstr_p, vmstate_p, vmarg_p);


#define X(x)		{ .handler = x },
#define S(s)		{ .id = s },
#define N(n)		{ .cell = (cell_ft) (n) },
#define L(x)		X(do_literal) N(x)

#define DIRECT_FORTH(nm)	vminstr_d nm[] = {
#define END_DIRECT		X(x_exit) };

#endif // DIRECT_H
