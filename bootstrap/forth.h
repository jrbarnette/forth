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

extern void report_exception(int, vmstate_ft *, char *);
extern void direct_execute(vmstate_ft *, vmip_ft);
extern void execute(vmstate_ft *, xt_ft);

#endif // FORTH_H
