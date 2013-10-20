/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <stdio.h>

#include "forth.h"

/*
 * format.c - Forth standard words for formatted output and number
 *   conversion.
 */

/*------  ------  ------  ------  ------  ------  ------  ------
  .                     6.1.0180 CORE                   27
  .R                    6.2.0210 CORE EXT               49
  BASE                  6.1.0750 CORE                   34
  DECIMAL               6.1.1170 CORE                   36
  HEX                   6.2.1660 CORE EXT               54
  U.                    6.1.2320 CORE                   46
  U.R                   6.2.2330 CORE EXT               57
  ------  ------  ------  ------  ------  ------  ------  ------
*/


#define MAXBASE			36
#define NUM_SIZE		(8 * CELL_SIZE + 1)

static char digits[MAXBASE] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

static char *
printunum(char *cp, cell_ft uval)
{
    cell_ft	base = DICT.base;

    *cp = '\0';
    do {
	cell_ft n = uval / base;
	char c;

	if (base <= MAXBASE) {
	    c = digits[uval % base];
	} else {
	    /* implementation defined */
	    c = '#';
	}
	*--cp = c;
	uval = n;
    } while (uval != 0);

    return cp;
}


/* ( n -- ) */
static vminstr_p
x_dot(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    char	tbuf[NUM_SIZE];
    char *	cp;
    snumber_ft	n;

    CHECK_POP(vm, 1);
    n = (snumber_ft) POP(vm);

    if (n < 0) {
	n = -n;
	fputc('-', stdout);
    }
    cp = printunum(&tbuf[sizeof (tbuf) - 1], n);
    fputs(cp, stdout);
    fputc(' ', stdout);

    return ip;
}


/* ( -- a-addr ) */
static vminstr_p
x_base(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) &DICT.base);
    return ip;
}


/* ( -- ) */
static vminstr_p
x_decimal(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    DICT.base = 10;
    return ip;
}


/* ( u -- ) */
static vminstr_p
x_udot(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    char	tbuf[NUM_SIZE];

    CHECK_POP(vm, 1);

    fputs(printunum(&tbuf[sizeof (tbuf) - 1], POP(vm)), stdout);
    fputc(' ', stdout);

    return ip;
}


/* ( n1 n2 -- ) */
static vminstr_p
x_dot_r(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    char	tbuf[NUM_SIZE];

    CHECK_POP(vm, 2);
    cell_ft *sp = SP(vm);
    snumber_ft r = PICK(sp, 0);
    snumber_ft n = PICK(sp, 1);
    SET_SP(vm, sp, 2);

    char sign = '+';
    if (n < 0) {
	n = -n;
	sign = '-';
	r--;
    }

    char *cp = printunum(&tbuf[sizeof (tbuf) - 1], n);
    int pad = cp - tbuf + r - sizeof (tbuf) + 1;
    while (pad-- > 0) {
	fputc(' ', stdout);
    }
    if (sign == '-') {
	fputc('-', stdout);
    }
    fputs(cp, stdout);

    return ip;
}


/* ( -- ) */
static vminstr_p
x_hex(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    DICT.base = 16;
    return ip;
}


/* ( u n -- ) */
static vminstr_p
x_udot_r(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    char	tbuf[NUM_SIZE];

    CHECK_POP(vm, 2);
    cell_ft *sp = SP(vm);
    snumber_ft n = PICK(sp, 0);
    cell_ft u = PICK(sp, 1);
    SET_SP(vm, sp, 2);

    char *cp = printunum(&tbuf[sizeof (tbuf) - 1], u);
    int pad = cp - tbuf + n - sizeof (tbuf) + 1;
    while (pad-- > 0) {
	fputc(' ', stdout);
    }
    fputs(cp, stdout);

    return ip;
}


defn_dt
format_defns[] =
{
    { define_name, ".",		x_dot },
    { define_name, "BASE",	x_base },
    { define_name, "DECIMAL",	x_decimal },
    { define_name, "U.",	x_udot },

    { define_name, ".R",	x_dot_r },
    { define_name, "HEX",	x_hex },
    { define_name, "U.R",	x_udot_r },
    { NULL }
};
