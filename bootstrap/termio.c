/*
 * Copyright 2009, by J. Richard Barnette
 */

#include <stdio.h>

#include "forth.h"

/*
 * termio.c - Standard Forth words for user terminal I/O.
 */

/* CR			6.1.0990 CORE, p. 36 */
/* ( -- ) */
cell_ft
x_cr(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    putc('\n', stdout);
    return tos;
}


/* EMIT			6.1.1320 CORE, p. 38 */
/* ( x -- ) */
cell_ft
x_emit(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    putc((char_ft) tos, stdout);
    return POP(vm);
}


/* SPACE		6.1.2220 CORE, p. 46 */
/* ( -- ) */
cell_ft
x_space(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    putc(' ', stdout);
    return tos;
}


/* SPACES		6.1.2230 CORE, p. 46 */
/* ( n -- ) */
cell_ft
x_spaces(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    snumber_ft	n = (snumber_ft) tos;

    CHECK_POP(vm, 1);
    while (n-- > 0) {
	putc(' ', stdout);
    }
    return POP(vm);
}


/* TYPE			6.1.2310 CORE, p. 47 */
/* ( c-addr u -- ) */
cell_ft
x_type(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    c_addr_ft	cp;

    CHECK_POP(vm, 2);
    cp = (c_addr_ft) POP(vm);
    if (tos > 0)
	fwrite(cp, sizeof (*cp), tos, stdout);
    return POP(vm);
}


defn_dt
termio_defns[] = {
    { define_name, "CR",	x_cr },
    { define_name, "EMIT",	x_emit },
    { define_name, "SPACE",	x_space },
    { define_name, "SPACES",	x_spaces },
    { define_name, "TYPE",	x_type },
    { NULL }
};

#if 0
    ACCEPT                6.1.0695 CORE                   33
    KEY                   6.1.1750 CORE                   41
    EXPECT                6.2.1390 CORE EXT               55
    QUERY                 6.2.2040 CORE EXT               57
    SPAN                  6.2.2240 CORE EXT               58
#endif
