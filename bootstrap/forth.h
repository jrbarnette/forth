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

extern void direct_execute(vmstate_ft *, vmip_ft);

#endif // FORTH_H
