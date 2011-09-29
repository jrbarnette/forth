/*
 * Copyright 2009, by J. Richard Barnette
 */

#include "forth.h"

/*
 * multops.c - Standard Forth words for double cell multiply
 *   and divide operations.
 */


/* * "star"		6.1.0090 CORE, p. 26 */
/* ( x1 x2 -- x ) */
static vminstr_p
x_star(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) * PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


defn_dt
multops_defns[] = {
    { define_name, "*",		x_star },
    { NULL }
};

#if 0
    */                    6.1.0100 CORE                   26
    */MOD                 6.1.0110 CORE                   26
    /                     6.1.0230 CORE                   28
    /MOD                  6.1.0240 CORE                   28
    FM/MOD                6.1.1561 CORE                   40
    M*                    6.1.1810 CORE                   42
    MOD                   6.1.1890 CORE                   43
    S>D                   6.1.2170 CORE                   45
    SM/REM                6.1.2214 CORE                   46
    UM*                   6.1.2360 CORE                   47
    UM/MOD                6.1.2370 CORE                   48
#endif
