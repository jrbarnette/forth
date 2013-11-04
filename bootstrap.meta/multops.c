/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include "forth.h"

/*
 * multops.c - Standard Forth words for single- and double cell
 *   multiply and divide operations.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// *                     6.1.0090 CORE                   26
// */                    6.1.0100 CORE                   26
// */MOD                 6.1.0110 CORE                   26
// /                     6.1.0230 CORE                   28
// /MOD                  6.1.0240 CORE                   28
// FM/MOD                6.1.1561 CORE                   39
// M*                    6.1.1810 CORE                   41
// MOD                   6.1.1890 CORE                   42
// SM/REM                6.1.2214 CORE                   45
// UM*                   6.1.2360 CORE                   46
// UM/MOD                6.1.2370 CORE                   46
//------  ------  ------  ------  ------  ------  ------  ------


#define HIGH_BIT	(~(~(cell_ft) 0 >> 1))
#define HALF_SHIFT	((cell_ft) (4 * CELL_SIZE))
#define HALF_MASK	(~(cell_ft) 0 >> HALF_SHIFT)

static void
dmult(cell_ft *sp, cell_ft n1, cell_ft n2, cell_ft sign)
{
    cell_ft n1_lo = n1 & HALF_MASK;
    cell_ft n1_hi = n1 >> HALF_SHIFT;
    cell_ft n2_lo = n2 & HALF_MASK;
    cell_ft n2_hi = n2 >> HALF_SHIFT;

    cell_ft mid1 = n1_hi * n2_lo;
    cell_ft mid2 = n1_lo * n2_hi;
    cell_ft hi = n1_hi * n2_hi + (mid1 >> HALF_SHIFT) +
		    (mid2 >> HALF_SHIFT);
    cell_ft tlo, lo;

    tlo = n1_lo * n2_lo;
    lo = tlo + (mid1 << HALF_SHIFT);
    hi += (lo < tlo);
    tlo = lo;
    lo = tlo + (mid2 << HALF_SHIFT);
    hi += (lo < tlo);

    PICK(sp, 1) = lo;
    PICK(sp, 0) = hi + sign;
}


/* ( x1 x2 -- x ) */
static vminstr_p
x_star(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) * PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( n1 n2 -- n ) */
static vminstr_p
x_slash(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    snumber_ft n1 = PICK(sp, 1);
    snumber_ft n2 = PICK(sp, 0);
    PICK(sp, 0) = (cell_ft) (n1 / n2);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( n1 n2 -- n3 n4 ) */
static vminstr_p
x_slash_mod(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    snumber_ft n1 = PICK(sp, 1);
    snumber_ft n2 = PICK(sp, 0);
    PICK(sp, 1) = (cell_ft) (n1 / n2);
    PICK(sp, 0) = (cell_ft) (n1 % n2);
    return ip;
}


/* ( n1 n2 -- d ) */
static vminstr_p
x_m_star(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    cell_ft n1 = PICK(sp, 1);
    cell_ft n2 = PICK(sp, 0);
    cell_ft s1 = -((snumber_ft) n1 >= 0);
    cell_ft s2 = -((snumber_ft) n2 >= 0);
    cell_ft sign = ((s1 & -n1) + (s2 & -n2)) & (s1 ^ s2);
    dmult(sp, n1, n2, sign);

    return ip;
}


/* ( n1 n2 -- n ) */
static vminstr_p
x_mod(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    cell_ft n1 = PICK(sp, 1);
    cell_ft n2 = PICK(sp, 0);
    PICK(sp, 0) = (cell_ft) (n1 % n2);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( u1 u2 -- ud ) */
static vminstr_p
x_u_m_star(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    cell_ft n1 = PICK(sp, 1);
    cell_ft n2 = PICK(sp, 0);
    dmult(sp, n1, n2, 0);

    return ip;
}


/* ( ud u1 -- u2 u3 ) */
static vminstr_p
x_u_m_slash_mod(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 3);
    cell_ft v = PICK(sp, 0);
    if (v == 0) {
	THROW(vm, -10);
    }
    cell_ft d_hi = PICK(sp, 1);
    cell_ft d_lo = PICK(sp, 2);

    cell_ft r_hi = d_hi % v;
    cell_ft factor = (HIGH_BIT / v) << 1;
    cell_ft factor_rem = (HIGH_BIT % v) << 1;
    cell_ft q_lo = d_lo / v + r_hi * factor;
    cell_ft rem = d_lo % v + r_hi * factor_rem;

    PICK(sp, 2) = q_lo + rem / v;
    PICK(sp, 1) = rem % v;
    SET_SP(vm, sp, 1);

    return ip;
}


DIRECT_FORTH(init_mult_prim) // {
    PRIM("*",		x_star)
    PRIM("/",		x_slash)
    PRIM("/MOD",	x_slash_mod)
    PRIM("M*",		x_m_star)
    PRIM("MOD",		x_mod)
    PRIM("UM*",		x_u_m_star)
    PRIM("UM/MOD",	x_u_m_slash_mod)
END_DIRECT // }

// */                    6.1.0100 CORE                   26
// */MOD                 6.1.0110 CORE                   26
// FM/MOD                6.1.1561 CORE                   39
// SM/REM                6.1.2214 CORE                   45
// UM/MOD                6.1.2370 CORE                   46
