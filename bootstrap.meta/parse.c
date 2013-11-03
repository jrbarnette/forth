/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <ctype.h>

#include "forth.h"

/*
 * parse.c - Standard Forth words for parsing.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// (                     6.1.0080 CORE                   26
// >IN                   6.1.0560 CORE                   31
// CHAR                  6.1.0895 CORE                   35
// SOURCE                6.1.2216 CORE                   45
//
// PARSE                 6.2.2008 CORE EXT               55
//------  ------  ------  ------  ------  ------  ------  ------


META_FORTH(init_parse_ops) // {
    XVARIABLE(">IN")
    L(3) CELLS ALLOT 
    XCOLON("SOURCE")
	INTERP( TO_IN CELL_PLUS ) LITERAL TWO_FETCH
    XSEMICOLON
    XCOLON("PARSE")		/* ( char "ccc<char>" -- c-addr u ) */
	TO_R SOURCE SWAP TO_IN FETCH CHARS PLUS TO_R
						/* ( R: delim c-addr ) */
	TO_IN FETCH MINUS L(-1)
	BEGIN ONE_PLUS TWO_DUP GREATER WHILE	/* ( end idx ) */
	    DUP CHARS TWO_R_FETCH
	    ROT PLUS C_FETCH			/* ( end idx delim c ) */
	    OVER L(' ') EQUALS IF
		SWAP DROP L(' '+1) MINUS L('~'-' ') U_LESS
	    ELSE
		EQUALS
	    THEN
	UNTIL					/* ( end idx ) */
	    SWAP DROP DUP ONE_PLUS
	THEN /* ( len idx ) */
	TO_IN PLUS_STORE R_FROM SWAP R_FROM DROP
    XSEMICOLON
END_META // }

/*
Q:  where should skip-delimiters (for parse-word) live?

clients of skip-delimiters:
    '                     6.1.0070 CORE                   25
    CHAR                  6.1.0895 CORE                   35
    POSTPONE              6.1.2033 CORE                   43
    WORD                  6.1.2450 CORE                   49
    [']                   6.1.2510 CORE                   50
    [CHAR]                6.1.2520 CORE                   50
    TO                    6.2.2295 CORE EXT               59
    [COMPILE]             6.2.2530 CORE EXT               60

but we can ignore these:
    : [CHAR] CHAR POSTPONE LITERAL ; IMMEDIATE
    : ['] ' POSTPONE LITERAL ; IMMEDIATE

    These should all rely on a common definition that does a lookup
    after parsing a name:
	' POSTPONE [COMPILE] TO
    ... which should also be used in interpret.  So those four
    belong in interpret.c.

    Only WORD and CHAR are different, and they can can reasonably be
    added to interpret.c, too.

    So...  it seems like skip-delimiters can go in interpret.c.
    We'd like PARSE to live with skip-delimiters...
*/
