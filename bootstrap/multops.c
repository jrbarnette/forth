/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include "forth.h"

/*
 * multops.c - Standard Forth words for single and double cell
 *   multiply and divide operations.
 */

/*------  ------  ------  ------  ------  ------  ------  ------
 * *                     6.1.0090 CORE                   26
 * /                     6.1.0230 CORE                   28
 * /MOD                  6.1.0240 CORE                   28
 * M*                    6.1.1810 CORE                   41
 * MOD                   6.1.1890 CORE                   42
 * UM*                   6.1.2360 CORE                   46
 * UM/MOD                6.1.2370 CORE                   46
 *------  ------  ------  ------  ------  ------  ------  ------
 */


// FIXME - missing the following:
// */                    6.1.0100 CORE                   26
// */MOD                 6.1.0110 CORE                   26
// FM/MOD                6.1.1561 CORE                   39
// SM/REM                6.1.2214 CORE                   45


#define HIGH_BIT	(~(~(cell_ft) 0 >> 1))
#define HALF_SHIFT	(4 * CELL_SIZE)
#define HI(x)		((cell_ft) (x) >> HALF_SHIFT)
#define HALF_MASK	HI(~(cell_ft) 0)
#define LO(x)		((cell_ft) (x) & HALF_MASK)


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
vminstr_p
x_star(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    PICK(sp, 1) = PICK(sp, 1) * PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( n1 n2 -- n ) */
vminstr_p
x_slash(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    snumber_ft n1 = PICK(sp, 1);
    snumber_ft n2 = PICK(sp, 0);
    if (n2 == 0) {
	THROW(vm, -10);
    }
    PICK(sp, 1) = (cell_ft) (n1 / n2);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( n1 n2 -- n3 n4 ) */
vminstr_p
x_slash_mod(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    snumber_ft n1 = PICK(sp, 1);
    snumber_ft n2 = PICK(sp, 0);
    if (n2 == 0) {
	THROW(vm, -10);
    }
    PICK(sp, 1) = (cell_ft) (n1 % n2);
    PICK(sp, 0) = (cell_ft) (n1 / n2);
    return ip;
}


/* ( n1 n2 -- d ) */
vminstr_p
x_m_star(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    cell_ft n1 = PICK(sp, 1);
    cell_ft n2 = PICK(sp, 0);
    cell_ft s1 = -((snumber_ft) n1 < 0);
    cell_ft s2 = -((snumber_ft) n2 < 0);
    cell_ft sign = ((s1 & -n2) + (s2 & -n1));
    dmult(sp, n1, n2, sign);

    return ip;
}


/* ( n1 n2 -- n ) */
vminstr_p
x_mod(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 2);
    cell_ft n1 = PICK(sp, 1);
    cell_ft n2 = PICK(sp, 0);
    if (n2 == 0) {
	THROW(vm, -10);
    }
    PICK(sp, 1) = (cell_ft) (n1 % n2);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( u1 u2 -- ud ) */
vminstr_p
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
vminstr_p
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

    cell_ft rem;
    cell_ft quot;
    if (d_hi == 0) {
	// This is the easy case.
	rem = d_lo % v;
	quot = d_lo / v;
    } else if (v <= HALF_MASK) {
	cell_ft u_hi = HI(d_lo);
	cell_ft u_lo = LO(d_lo);
	cell_ft u = ((d_hi % v) << HALF_SHIFT) + u_hi;
	rem = u % v;
	quot = u / v;
	u = (rem << HALF_SHIFT) + u_lo;
	rem = u % v;
	quot = (quot << HALF_SHIFT) + u / v;
    } else if (v > HIGH_BIT) {
	// This is Algorithm D from Knuth Vol. 2, "Seminumerical
	// Algorithms".  We're using "digits" of a half cell,
	// meaning the algorithm parameters are:
	//   b = HALF_MASK + 1 (the arithmetic base)
	//   n = 2 (the number of digits in the divisor)
	//   m = 2 (m + n is the number of digits in the dividend)
	//
	// The code is specialized for things that are known at
	// compile time:
	//  a) Because we know that v > HIGH_BIT we can skip steps
	//     D1 and D8 (normalize/unnormalize).
	//  b) Since n = 2, a number of the intermediate results
	//     can be stored as single-cell values.
	//  c) Since n = 2, we can skip steps D5 and D6 (test
	//     remainder and add back).
	//  d) Since we only return a single cell for the quotient,
	//     we throw away the first quotient digit by starting
	//     with rem = d_hi % v.
	rem = d_hi;
	quot = 0;
	if (rem >= v) {
	    rem -= v;
	}
	cell_ft v_hi = HI(v);
	cell_ft v_lo = LO(v);
	cell_ft u[] = { HI(d_lo), LO(d_lo) };
	for (int i = 0; i < sizeof (u) / sizeof (u[0]); i++) {
	    cell_ft rhat;
	    cell_ft qhat;
	    // N.B. If HI(rem) = v_hi = HALF_MASK, then qhat must be
	    // HALF_MASK, and the calculation of qv_total below would
	    // overflow a single cell.  So, we handle that case
	    // specially.
	    if (HI(rem) >= v_hi) {
		if (v_hi == HALF_MASK) {
		    cell_ft rem_hi = HALF_MASK + LO(rem) - v_lo;
		    rem = (rem_hi << HALF_SHIFT) + v_lo + u[i];
		    quot = (quot << HALF_SHIFT) + HALF_MASK;
		    continue;
		}
		rhat = rem - (v_hi << HALF_SHIFT) + v_hi;
		qhat = HALF_MASK;
	    } else {
		rhat = rem % v_hi;
		qhat = rem / v_hi;
	    }
	    cell_ft qv_lo = qhat * v_lo;
	    cell_ft qv_total = (rhat << HALF_SHIFT) + u[i];
	    while (qv_lo > qv_total) {
		qhat--;
		qv_lo -= v_lo;
		qv_total += v_hi << HALF_SHIFT;
	    }
	    rem = qv_total - qv_lo;
	    quot = (quot << HALF_SHIFT) + qhat;
	}
    } else {
	// To use Knuth's Algorithm D in this case would require the
	// normalize/unnormalize steps.  That's kind of expensive, so
	// here's a formula that works so long as (v <= HIGH_BIT).
	cell_ft r_hi = d_hi % v;
	cell_ft rem_part = d_lo % v + r_hi * ((HIGH_BIT % v) << 1);
	cell_ft q_part = d_lo / v + r_hi * ((HIGH_BIT / v) << 1);
	rem = rem_part % v;
	quot = q_part + rem_part / v;
    }
    PICK(sp, 2) = rem;
    PICK(sp, 1) = quot;
    SET_SP(vm, sp, 1);

    return ip;
}
