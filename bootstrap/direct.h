/*
 * Copyright 2017, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef DIRECT_H
#define DIRECT_H

#include <stdddef.h>

#include "forth.h"
#include "dictionary.h"

/*
 * Support for dictionary initialization by interpreting direct-threaded
 * code stored in arrays.
 */

// Most of these are unused now, but if we ever need to hand-write
// direct code, this is how it's done.
#define X(x)           { .handler = x },
#define S(s)           { .id = s },
#define N(n)           { .cell = (cell_ft) (n) },
#define L(x)           X(do_literal) N(x)
#define PRIM(nm, hdlr) X(i_addname) S(nm) { .handler = hdlr },
#define FLAGS(f)       X(i_setflags) { .cell = NAME_TYPE_ ## f },
#define META           X(meta_interpret)
#define DIRECT         S(NULL)

#define DIRECT_FORTH(nm)	vminstr_d nm[] = {
#define END_DIRECT		X(x_exit) };

#endif // DIRECT_H
