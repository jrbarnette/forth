/*
 * Copyright 2011, by J. Richard Barnette
 */

#include <stdio.h>

#include "forth.h"

/*
 * format.c - Forth standard words for formatted output and number
 *   conversion.
 */

#define MAXBASE			36

static char digits[MAXBASE] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

static char *
printunum(char *cp, cell_ft uval, cell_ft base)
{
    *cp = '\0';
    *--cp = ' ';
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


/* . "dot"		6.1.0180 CORE, p. 27 */
/* ( n -- ) */
static vminstr_p
x_dot(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    char	tbuf[CELL_SIZE * 8 + 3];
    char *	cp;
    char	sign = '+';
    snumber_ft	n;

    CHECK_POP(vm, 1);
    n = (snumber_ft) POP(vm);

    if (n < 0) {
	n = -n;
	sign = '-';
    }
    cp = printunum(&tbuf[sizeof (tbuf) - 1], n, DICT.base);
    if (sign == '-') {
	*--cp = sign;
    }
    fputs(cp, stdout);

    return ip;
}


/* BASE			6.1.0750 CORE, p. 34 */
/* ( -- a-addr ) */
static vminstr_p
x_base(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) &DICT.base);
    return ip;
}


/* DECIMAL		6.1.1170 CORE, p. 36 */
/* ( -- ) */
static vminstr_p
x_decimal(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    DICT.base = 10;
    return ip;
}


/* U.			6.1.2320 CORE, p. 47 */
/* ( u -- ) */
static vminstr_p
x_udot(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    char	tbuf[CELL_SIZE * 8 + 3];

    CHECK_POP(vm, 1);

    fputs(printunum(&tbuf[sizeof (tbuf) - 1], POP(vm), DICT.base), stdout);

    return ip;
}


/* HEX			6.2.1660 CORE EXT, p. 55 */
/* ( -- ) */
static vminstr_p
x_hex(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    DICT.base = 16;
    return ip;
}


defn_dt
format_defns[] =
{
    { define_name, ".",		x_dot },
    { define_name, "BASE",	x_base },
    { define_name, "DECIMAL",	x_decimal },
    { define_name, "U.",	x_udot },
    { define_name, "HEX",	x_hex },
    { NULL }
};

#if 0
    #                     6.1.0030 CORE                   25
    #>                    6.1.0040 CORE                   25
    #S                    6.1.0050 CORE                   25
    <#                    6.1.0490 CORE                   31
    >NUMBER               6.1.0570 CORE                   31
    HOLD                  6.1.1670 CORE                   40
    SIGN                  6.1.2210 CORE                   46
    .R                    6.2.0210 CORE EXT               51
    CONVERT               6.2.0970 CORE EXT               54
    U.R                   6.2.2330 CORE EXT               59
#endif
