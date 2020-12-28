/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include "forth.h"

/*
 * multops.c - Standard Forth words for single and double cell
 *   multiply and divide operations.
 */

// N.B. The names "*/" and "*/MOD" can't appear in /* */ comments.
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


static void
ddivide(cell_ft *sp, cell_ft d_hi, cell_ft d_lo, cell_ft v)
{
    cell_ft rem;
    cell_ft quot;
    if (d_hi == 0) {
	// This is the easy case.
	rem = d_lo % v;
	quot = d_lo / v;
    } else if (v <= HALF_MASK) {
	cell_ft u = ((d_hi % v) << HALF_SHIFT) + HI(d_lo);
	rem = u % v;
	quot = u / v;
	u = (rem << HALF_SHIFT) + LO(d_lo);
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
	//  a) The condition v > HIGH_BIT means that v1 > b / 2, so
	//     we can skip steps D1 and D8 (normalize/unnormalize).
	//  b) Since n = 2, a number of intermediate results can be
	//     stored as single-cell values.
	//  c) The value of qhat will be exact when n = 2, so we
	//     skip steps D5 and D6 (test remainder and add back).
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
    PICK(sp, 1) = rem;
    PICK(sp, 0) = quot;
}


static void
dmult_divide(cell_ft *sp, cell_ft n1, cell_ft n2, cell_ft n3)
{
    cell_ft s1 = -((snumber_ft) n1 < 0);
    cell_ft s2 = -((snumber_ft) n2 < 0);
    cell_ft s3 = -((snumber_ft) n3 < 0);
    if (s1) {
	n1 = -n1;
    }
    if (s2) {
	n2 = -n2;
    }
    if (s3) {
	n3 = -n3;
    }

    dmult(sp, n1, n2, 0);
    ddivide(sp, PICK(sp, 0), PICK(sp, 1), n3);
    if (s1 ^ s2) {
	PICK(sp, 1) = -PICK(sp, 1);
    }
    if (s1 ^ s2 ^ s3) {
	PICK(sp, 0) = -PICK(sp, 0);
    }
}


/* ( x1 x2 -- x ) */
PRIM_HDLR(x_star)
{
    CHECK_POP(vm, 2);

    cell_ft *sp = SP(vm);
    PICK(sp, 1) = PICK(sp, 1) * PICK(sp, 0);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( n1 n2 n3 -- n4 ) */
PRIM_HDLR(x_star_slash)
{
    CHECK_POP(vm, 3);

    cell_ft *sp = SP(vm);
    cell_ft n1 = PICK(sp, 2);
    cell_ft n2 = PICK(sp, 1);
    cell_ft n3 = PICK(sp, 0);

    CHECK(vm, (n3 == 0), -10);

    dmult_divide(sp, n1, n2, n3);
    PICK(sp, 2) = PICK(sp, 0);
    SET_SP(vm, sp, 2);
    return ip;
}


/* ( n1 n2 n3 -- n4 n5 ) */
PRIM_HDLR(x_star_slash_mod)
{
    CHECK_POP(vm, 3);

    cell_ft *sp = SP(vm);
    cell_ft n1 = PICK(sp, 2);
    cell_ft n2 = PICK(sp, 1);
    cell_ft n3 = PICK(sp, 0);

    CHECK(vm, (n3 == 0), -10);

    dmult_divide(SET_SP(vm, sp, 1), n1, n2, n3);
    return ip;
}


/* ( n1 n2 -- n ) */
PRIM_HDLR(x_slash)
{
    CHECK_POP(vm, 2);

    cell_ft *sp = SP(vm);
    snumber_ft n1 = PICK(sp, 1);
    snumber_ft n2 = PICK(sp, 0);

    CHECK(vm, (n2 == 0), -10);

    PICK(sp, 1) = (cell_ft) (n1 / n2);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( n1 n2 -- n3 n4 ) */
PRIM_HDLR(x_slash_mod)
{
    CHECK_POP(vm, 2);

    cell_ft *sp = SP(vm);
    snumber_ft n1 = PICK(sp, 1);
    snumber_ft n2 = PICK(sp, 0);

    CHECK(vm, (n2 == 0), -10);

    PICK(sp, 1) = (cell_ft) (n1 % n2);
    PICK(sp, 0) = (cell_ft) (n1 / n2);
    return ip;
}


/* ( d n1 -- n2 n3 ) */
PRIM_HDLR(x_f_m_slash_mod)
{
    CHECK_POP(vm, 3);

    cell_ft *sp = SP(vm);
    cell_ft d_hi = PICK(sp, 1);
    cell_ft d_lo = PICK(sp, 2);
    cell_ft v = PICK(sp, 0);

    CHECK(vm, (v == 0), -10);

    cell_ft s_d = -((snumber_ft) d_hi < 0);
    cell_ft s_v = -((snumber_ft) v < 0);

    if (s_d) {
	if (d_lo) {
	    d_lo = -d_lo;
	    d_hi = ~d_hi;
	} else {
	    d_hi = -d_hi;
	}
    }
    if (s_v) {
	v = -v;
    }
    if (s_d != s_v) {
	cell_ft t_lo = d_lo;
	d_lo += v - 1;
	d_hi += (d_lo < t_lo);
    }

    sp = SET_SP(vm, sp, 1);
    ddivide(sp, d_hi, d_lo, v);
    if (s_d != s_v) {
	PICK(sp, 1) -= v - 1;
	PICK(sp, 0) = -PICK(sp, 0);
    }
    if (s_d) {
	PICK(sp, 1) = -PICK(sp, 1);
    }

    return ip;
}


/* ( n1 n2 -- d ) */
PRIM_HDLR(x_m_star)
{
    CHECK_POP(vm, 2);

    cell_ft *sp = SP(vm);
    cell_ft n1 = PICK(sp, 1);
    cell_ft n2 = PICK(sp, 0);
    cell_ft s1 = -((snumber_ft) n1 < 0);
    cell_ft s2 = -((snumber_ft) n2 < 0);
    cell_ft sign = ((s1 & -n2) + (s2 & -n1));
    dmult(sp, n1, n2, sign);

    return ip;
}


/* ( n1 n2 -- n ) */
PRIM_HDLR(x_mod)
{
    CHECK_POP(vm, 2);

    cell_ft *sp = SP(vm);
    snumber_ft n1 = PICK(sp, 1);
    snumber_ft n2 = PICK(sp, 0);

    CHECK(vm, (n2 == 0), -10);

    PICK(sp, 1) = (cell_ft) (n1 % n2);
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( d n1 -- n2 n3 ) */
PRIM_HDLR(x_s_m_slash_rem)
{
    CHECK_POP(vm, 3);

    cell_ft *sp = SP(vm);
    cell_ft d_lo = PICK(sp, 2);
    cell_ft d_hi = PICK(sp, 1);
    cell_ft v = PICK(sp, 0);

    CHECK(vm, (v == 0), -10);

    cell_ft s_d = -((snumber_ft) d_hi < 0);
    cell_ft s_v = -((snumber_ft) v < 0);

    if (s_d) {
	if (d_lo) {
	    d_lo = -d_lo;
	    d_hi = ~d_hi;
	} else {
	    d_hi = -d_hi;
	}
    }
    if (s_v) {
	v = -v;
    }

    sp = SET_SP(vm, sp, 1);
    ddivide(sp, d_hi, d_lo, v);
    if (s_d != s_v) {
	PICK(sp, 0) = -PICK(sp, 0);
    }
    if (s_d) {
	PICK(sp, 1) = -PICK(sp, 1);
    }

    return ip;
}


/* ( u1 u2 -- ud ) */
PRIM_HDLR(x_u_m_star)
{
    CHECK_POP(vm, 2);

    cell_ft *sp = SP(vm);
    cell_ft n1 = PICK(sp, 1);
    cell_ft n2 = PICK(sp, 0);
    dmult(sp, n1, n2, 0);

    return ip;
}


/* ( ud u1 -- u2 u3 ) */
PRIM_HDLR(x_u_m_slash_mod)
{
    CHECK_POP(vm, 3);

    cell_ft *sp = SP(vm);
    cell_ft d_lo = PICK(sp, 2);
    cell_ft d_hi = PICK(sp, 1);
    cell_ft v = PICK(sp, 0);

    CHECK(vm, (v == 0), -10);

    ddivide(SET_SP(vm, sp, 1), d_hi, d_lo, v);

    return ip;
}
