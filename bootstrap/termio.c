/*
 * Copyright 2008, by J. Richard Barnette
 */

#include "forth.h"

/* . ( n -- ) */
cell_ft
x_dot(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    char	tbuf[CELL_SIZE * 8 + 2];
    char *	cp;
    cell_ft	base;

    CHECK_POP(vm, 1);

    base = *(cell_ft *) x_base(tos, vm, ignore);

    cp = &tbuf[CELL_SIZE * 8 + 1];
    *cp = ' ';
    do {
	int dig = tos % base;

	if (dig < 10) {
	    *--cp = '0' + dig;
	} else if (dig < 36) {
	    *--cp = 'A' + dig - 10;
	} else {
	    /* THROW */
	    *--cp = '#';
	}
	tos /= base;
    } while (tos != 0);

    return POP(vm);
}

#if 0
    #                     6.1.0030 CORE                   25
    #>                    6.1.0040 CORE                   25
    #S                    6.1.0050 CORE                   25
    ."                    6.1.0190 CORE                   28
    <#                    6.1.0490 CORE                   31
    >IN                   6.1.0560 CORE                   31
    CR                    6.1.0990 CORE                   36
    DECIMAL               6.1.1170 CORE                   36
    EMIT                  6.1.1320 CORE                   38
    HOLD                  6.1.1670 CORE                   40
    KEY                   6.1.1750 CORE                   41
    SIGN                  6.1.2210 CORE                   46
    SOURCE                6.1.2216 CORE                   46
    SPACE                 6.1.2220 CORE                   46
    SPACES                6.1.2230 CORE                   46
    TYPE                  6.1.2310 CORE                   47
    U.                    6.1.2320 CORE                   47
    .R                    6.2.0210 CORE EXT               51
    CONVERT               6.2.0970 CORE EXT               54
    HEX                   6.2.1660 CORE EXT               55
    PARSE                 6.2.2008 CORE EXT               57
    REFILL                6.2.2125 CORE EXT               57
    RESTORE-INPUT         6.2.2148 CORE EXT               57
    SAVE-INPUT            6.2.2182 CORE EXT               58
    SOURCE-ID             6.2.2218 CORE EXT               58
    U.R                   6.2.2330 CORE EXT               59
#endif

static defn_dt
termio_defs[] = {
    { ".",    x_dot },			/* 6.1.0180 CORE p. 27 */
#if 0
    { "BASE", x_base },			/* 6.1.0750 CORE p. 34 */
#endif
};
