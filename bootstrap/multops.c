/*
 * Copyright 2011, by J. Richard Barnette
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

#if 0
/* "star-slash"         6.1.0100 CORE, p. 26 */
/* ( n1 n2 n3 -- n ) */
static vminstr_p
x_star_slash(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 3);
    snumber_ft n1 = PICK(sp, 2);
    snumber_ft n2 = PICK(sp, 1);
    snumber_ft n3 = PICK(sp, 0);
    PICK(sp, 2) = (cell_ft) (((sdnumber_ft) n1 * n2) / n3);
    SET_SP(vm, sp, 2);
    return ip;
}

/* "star-slash-mod"     6.1.0110 CORE, p. 26 */
/* ( n1 n2 n3 -- n4 n5 ) */
static vminstr_p
x_star_slash_mod(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 3);
    snumber_ft n1 = PICK(sp, 2);
    snumber_ft n2 = PICK(sp, 1);
    snumber_ft n3 = PICK(sp, 0);
    sdnumber_ft tprod = (sdnumber_ft) n1 * n2;
    PICK(sp, 1) = (cell_ft) (tprod / n3);
    PICK(sp, 2) = (cell_ft) (tprod % n3);
    SET_SP(vm, sp, 1);
    return ip;
}
#endif

/* /                    6.1.0230 CORE, p. 28 */
/* ( n1 n2 -- n ) */
static vminstr_p
x_slash(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    snumber_ft n1 = PICK(sp, 1);
    snumber_ft n2 = PICK(sp, 0);
    PICK(sp, 0) = (cell_ft) (n1 / n2);
    SET_SP(vm, sp, 1);
    return ip;
}

/* /MOD                 6.1.0240 CORE, p. 28 */
/* ( n1 n2 -- n3 n4 ) */
static vminstr_p
x_slash_mod(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    snumber_ft n1 = PICK(sp, 1);
    snumber_ft n2 = PICK(sp, 0);
    PICK(sp, 1) = (cell_ft) (n1 / n2);
    PICK(sp, 0) = (cell_ft) (n1 % n2);
    return ip;
}

#if 0
/* M*                   6.1.1810 CORE, p. 41 */
/* ( n1 n2 -- d ) */
static vminstr_p
x_m_star(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    snumber_ft n1 = PICK(sp, 1);
    snumber_ft n2 = PICK(sp, 0);
    dsnumber_ft d = (dsnumber_ft) n1 * n2;
    PICK(sp, 1) = (cell_ft) d;
    PICK(sp, 0) = (cell_ft) (d >> 8 * sizeof (cell_ft));
    return ip;
}
#endif

/* MOD                  6.1.1890 CORE, p. 43 */
/* ( n1 n2 -- n ) */
static vminstr_p
x_mod(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    snumber_ft n1 = PICK(sp, 1);
    snumber_ft n2 = PICK(sp, 0);
    PICK(sp, 0) = (cell_ft) (n1 % n2);
    SET_SP(vm, sp, 1);
    return ip;
}

/* S>D                  6.1.2170 CORE, p. 45 */
/* ( n -- d ) */
static vminstr_p
x_s_to_d(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    CHECK_PUSH(vm, 1);
    snumber_ft n = PICK(sp, 0);
    PUSH(vm, n >> (8 * sizeof (snumber_ft) - 1));
    return ip;
}

#if 0
/* UM*                  6.1.2360 CORE, p. 47 */
/* ( u1 u2 -- ud ) */
static vminstr_p
x_u_m_star(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    cell_ft u1 = PICK(sp, 1);
    cell_ft u2 = PICK(sp, 0);
    dcell_ft ud = (dcell_ft) u1 * u2;
    PICK(sp, 1) = (cell_ft) ud;
    PICK(sp, 0) = (cell_ft) (ud >> 8 * sizeof (cell_ft));
    return ip;
}
#endif


defn_dt
multops_defns[] = {
    { define_name, "*",		x_star },
#if 0
    { define_name, "*/",	x_star_slash },
    { define_name, "*/MOD",	x_star_slash_mod },
#endif
    { define_name, "/",		x_slash },
    { define_name, "/MOD",	x_slash_mod },
#if 0
    { define_name, "M*",	x_m_star },
#endif
    { define_name, "MOD",	x_mod },
    { define_name, "S>D",	x_s_to_d },
#if 0
    { define_name, "UM*",	x_u_m_star },
#endif
    { NULL }
};

#if 0
    FM/MOD                6.1.1561 CORE                   40
    SM/REM                6.1.2214 CORE                   46
    UM/MOD                6.1.2370 CORE                   48
#endif
