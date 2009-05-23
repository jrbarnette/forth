/*
 * Copyright 2009, by J. Richard Barnette
 */

#include <stdio.h>

#include "forth.h"

/* . "dot"		6.1.0180 CORE, p. 27 */
/* ( n -- ) */
cell_ft
x_dot(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    char	tbuf[CELL_SIZE * 8 + 3];
    char *	cp;
    cell_ft	base;
    char	sign = '+';

    CHECK_POP(vm, 1);

    base = DICT.base;

    cp = &tbuf[sizeof (tbuf) - 1];
    *cp = '\0';
    *--cp = ' ';
    if ((snumber_ft) tos < 0) {
	tos = -tos;
	sign = '-';
    }
    do {
	char c;
	int dig = tos % base;
	tos /= base;

	if (dig < 10) {
	    c = '0' + dig;
	} else if (dig < 36) {
	    c = 'A' - 10 + dig;
	} else {
	    /* implementation defined */
	    c = '#';
	}
	*--cp = c;
    } while (tos != 0);
    if (sign == '-') {
	*--cp = sign;
    }
    fputs(cp, stdout);

    return POP(vm);
}


/* BASE			6.1.0750 CORE, p. 34 */
/* ( -- a-addr ) */
cell_ft
x_base(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, tos);
    return (cell_ft) &DICT.base;
}


/* DECIMAL		6.1.1170 CORE, p. 36 */
/* ( -- ) */
cell_ft
x_decimal(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    DICT.base = 10;
    return tos;
}


defn_dt
format_defns[] =
{
    { define_name, ".",		x_dot },
    { define_name, "BASE",	x_base },
    { define_name, "DECIMAL",	x_decimal },
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
    U.                    6.1.2320 CORE                   47
    .R                    6.2.0210 CORE EXT               51
    HEX                   6.2.1660 CORE EXT               55
    U.R                   6.2.2330 CORE EXT               59
#endif
