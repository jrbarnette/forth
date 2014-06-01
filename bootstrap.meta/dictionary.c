/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>

#include "forth.h"

/*
 * dictionary.c - C internal functions and standard Forth words for
 *   dictionary data space.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// ,                     6.1.0150 CORE                   27
// ALIGN                 6.1.0705 CORE                   33
// ALLOT                 6.1.0710 CORE                   33
// C,                    6.1.0860 CORE                   34
// HERE                  6.1.1650 CORE                   39
//
// PAD                   6.2.2000 CORE EXT               55
// UNUSED                6.2.2395 CORE EXT               57
//------  ------  ------  ------  ------  ------  ------  ------

union dict dictionary;

/*
 * Align the current HERE pointer.
 */
void
align(void)
{
    addr_ft	h = DICT.here;
    cell_ft	n = (DICT_END - h) & (CELL_SIZE - 1);
    addr_ft	nh = h + n;

    assert(nh < DICT_END);
    DICT.here = nh;
}

/*
 * Allocate "n" character units of dictionary space.  Return a
 * pointer to the allocated space.
 */
addr_ft
allot(size_t n)
{
    addr_ft	h = DICT.here;
    addr_ft	nh = h + n;

    assert(nh < DICT_END);
    DICT.here = nh;
    return h;
}


/*
 * Allot and initialize one cell.
 */
void
comma(cell_ft n)
{
    *(a_addr_ft) allot(CELL_SIZE) = n;
}


vminstr_p
i_comma(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    comma(ip->cell);
    return ip + 1;
}


vminstr_p
init_dictionary(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    DICT.here = DICT_START + sizeof (DICT);
    return ip;
}


META_FORTH(init_dictionary_ops) // {
    XCOLON("HERE") L(&DICT.here) FETCH XSEMICOLON
    XCOLON("ALLOT")
	L(&DICT.here) FETCH PLUS L(&DICT.here) STORE
    XSEMICOLON
    XCOLON("ALIGN") HERE ALIGNED L(&DICT.here) STORE XSEMICOLON
    XCOLON(",") HERE L(CELL_SIZE) ALLOT STORE XSEMICOLON
    XCOLON("C,") HERE L(CHAR_SIZE) ALLOT C_STORE XSEMICOLON
END_META // }

// PAD                   6.2.2000 CORE EXT               55
// UNUSED                6.2.2395 CORE EXT               57
