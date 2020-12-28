/*
 * Copyright 2015, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef FORTH_H
#define FORTH_H

#include "ftypes.h"
#include "vmstate.h"
#include "xtoken.h"
#include "vmcheck.h"
#include "prim.h"

/*
 * C utility functions that are intrinsic to the inner VM (both
 * direct and indirect threaded).
 */

extern void report_exception(int, vmstate_p, char *);
extern void direct_execute(vmstate_p, vminstr_p);
extern void execute(vmstate_p, xt_ft);

#endif // FORTH_H
