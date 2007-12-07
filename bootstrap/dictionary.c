/*
 * Copyright 2007, by J. Richard Barnette
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"
#include "dictionary.h"

#define	HERE		dictionary.here
#define NAME_ALIGNMENT	sizeof (cell_t)
#define ALIGN(n)	((void) HERE = (HERE + (n)-1) & -(n))
#define NAUNITS(n)	(((n) + sizeof (addr_unit_ft) - 1) / sizeof (addr_unit_ft))

static dictionary_t dictionary;

/* -------------------------------------------------------------- */
addr_ft
allot(size_t n)
{
    cell_ft	h = HERE;
    cell_ft	nh = HERE + NAUNITS(n)

    if (nh >= DICTIONARY_SIZE) {
	fprintf(stderr, "out of dictionary space\n");
	abort();
    }
    HERE = nh;

    return dictionary.dict + h;
}

/* -------------------------------------------------------------- */
/* ( n -- ) */
static cell_t
x_allot(addr_ft ignore, cell_ft tos, vmstate_p vm)
{
    CHECK_UNDERFLOW(vm, 1);
    HERE += tos;

    if (HERE >= DICTIONARY_SIZE) {
	fprintf(stderr, "out of dictionary space\n");
	abort();	/* XXX -8 THROW */
    }

    return POP(vm);
}

/* -------------------------------------------------------------- */
xt_ft
lookup(char *n)
{
    name_p	cur;
    size_t	len = strlen(n);

    for (cur = dictionary.namelist; cur != NULL; cur = cur->prev) {
	if (len != cur->len)
	    continue;
	if (memcmp(n, cur->ident, len) != 0)
	    continue;

	return (xt_ft) ((char *) cur + NAME_CFA_OFFSET(cur))
    }

    return NULL;
}

/* -------------------------------------------------------------- */
name_p
addname(char *n, vminstr_fn cfdata)
{
    name_p	cur;
    size_t	len = strlen(n);
    xt_ft	xtok;

    if (len == 0) {
	fprintf(stderr, "empty name\n");
	abort();
    } else if (len > NAME_MAX_LENGTH) {
	fprintf(stderr, "name too long\n");
	abort();
    }

    cur = (name_p)ALIGNED(dictionary.here);
    cur->prev = dictionary.namelist;
    cur->len = len;
    (void) memcpy(cur->ident, n, len);
    xtok = (xt_ft) ((char *) cur + NAME_CFA_OFFSET(cur))
    xtok->handler = cfdata;
    dictionary.here = (cell_ft) xtok->data;

    return dictionary.namelist = cur;
}
