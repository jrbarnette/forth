/*
 * Copyright 2017, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef DIRECT_H
#define DIRECT_H

#include <stddef.h>

#include "forth.h"

/*
 * Support for dictionary initialization by interpreting direct-threaded
 * code stored in arrays.
 */

#define X(x)		{ .handler = x },
#define S(s)		{ .id = s },
#define N(n)		{ .cell = (cell_ft) (n) },
#define L(x)		X(do_literal) N(x)

#define DIRECT_FORTH(nm)	vminstr_d nm[] = {
#define END_DIRECT		X(x_exit) };

#endif // DIRECT_H
