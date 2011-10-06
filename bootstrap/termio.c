/*
 * Copyright 2011, by J. Richard Barnette
 */

#include <stdio.h>

#include "forth.h"

/*
 * termio.c - Standard Forth words for user terminal I/O.
 */

/* CR			6.1.0990 CORE, p. 36 */
/* ( -- ) */
static vminstr_p
x_cr(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    putc('\n', stdout);
    return ip;
}


/* EMIT			6.1.1320 CORE, p. 38 */
/* ( x -- ) */
static vminstr_p
x_emit(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    putc((char_ft) POP(vm), stdout);
    return ip;
}


/* SPACE		6.1.2220 CORE, p. 46 */
/* ( -- ) */
static vminstr_p
x_space(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    putc(' ', stdout);
    return ip;
}


/* SPACES		6.1.2230 CORE, p. 46 */
/* ( n -- ) */
static vminstr_p
x_spaces(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    snumber_ft	n;

    CHECK_POP(vm, 1);
    n = POP(vm);
    while (n-- > 0) {
	putc(' ', stdout);
    }
    return ip;
}


/* TYPE			6.1.2310 CORE, p. 47 */
/* ( c-addr u -- ) */
static vminstr_p
x_type(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    c_addr_ft	cp;
    size_t	count;

    CHECK_POP(vm, 2);
    count = (size_t) POP(vm);
    cp = (c_addr_ft) POP(vm);
    if (count != 0)
	fwrite(cp, sizeof (*cp), count, stdout);
    return ip;
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
