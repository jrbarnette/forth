/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "forth.h"

/*
 * names.c - C internal functions and standard Forth words for
 *   dictionary name space.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// '                     6.1.0070 CORE                   25
// :                     6.1.0450 CORE                   30
// ;                     6.1.0460 CORE                   30
// >BODY                 6.1.0550 CORE                   31
// CONSTANT              6.1.0950 CORE                   35
// CREATE                6.1.1000 CORE                   36
// DOES>                 6.1.1250 CORE                   37
// IMMEDIATE             6.1.1710 CORE                   40
// VARIABLE              6.1.2410 CORE                   47
//
// FORTH-WORDLIST     16.6.1.1595 SEARCH                119
// SEARCH-WORDLIST    16.6.1.2192 SEARCH                120
//------  ------  ------  ------  ------  ------  ------  ------


/*
 * Look up a definition in the dictionary, and return its execution
 * token.  Return NULL if not found.
 */
name_p
lookup(char *id, size_t len)
{
    name_p	cur;

    assert(len != 0);

    for (cur = DICT.forth_wordlist; cur != NULL; cur = cur->prev) {
	int i;

	if (len != NAME_LENGTH(cur))
	    continue;

	i = 0;
	while (toupper(id[i]) == toupper(cur->ident[i])) {
	    i++;
	    if (i == len)
		return cur;
	}
    }

    return NULL;
}


vminstr_p
i_addname(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    char *id = ip[0].str;
fprintf(stderr, "add %s\n", id);
    cell_ft len = strlen(ip[0].str);
    assert(0 < len && len < NAME_MAX_LENGTH);

    align();
    name_p name = (name_p) allot(NAME_SIZE(len) + CELL_SIZE);
    name->prev = DICT.forth_wordlist;
    name->flags = len;
    (void) memcpy(name->ident, id, len);

    xt_ft xtok = NAME_XT(name);
    xtok->handler = ip[1].handler;
    assert(DICT.here == xtok[1].arg->data);
    DICT.forth_wordlist = name;

    return ip + 2;
}


vminstr_p
i_setflags(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    NAME_SET_TYPE(DICT.forth_wordlist, ip[0].cell);
    return ip + 1;
}


vminstr_p
init_names(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    DICT.forth_wordlist = NULL;
    return ip;
}


// '                     6.1.0070 CORE                   25
// :                     6.1.0450 CORE                   30
// ;                     6.1.0460 CORE                   30
// >BODY                 6.1.0550 CORE                   31
// CONSTANT              6.1.0950 CORE                   35
// CREATE                6.1.1000 CORE                   36
// DOES>                 6.1.1250 CORE                   37
// IMMEDIATE             6.1.1710 CORE                   40
// VARIABLE              6.1.2410 CORE                   47
// FORTH-WORDLIST     16.6.1.1595 SEARCH                119
// SEARCH-WORDLIST    16.6.1.2192 SEARCH                120
