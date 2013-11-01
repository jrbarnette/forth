/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <stdio.h>

#include "forth.h"

/*
 * terminal.c - Standard Forth words for terminal I/O.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// .                     6.1.0180 CORE                   27
// ."                    6.1.0190 CORE                   28
// ACCEPT                6.1.0695 CORE                   33
// CR                    6.1.0990 CORE                   36
// EMIT                  6.1.1320 CORE                   38
// KEY                   6.1.1750 CORE                   40
// SPACE                 6.1.2220 CORE                   45
// SPACES                6.1.2230 CORE                   45
// TYPE                  6.1.2310 CORE                   46
// U.                    6.1.2320 CORE                   46
//
// .R                    6.2.0210 CORE EXT               49
// U.R                   6.2.2330 CORE EXT               57
//------  ------  ------  ------  ------  ------  ------  ------

/* ( x -- ) */
static vminstr_p
x_emit(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    char_ft c = (char_ft) POP(vm);
    putc(c, stdout);
    return ip;
}


DIRECT_FORTH(init_terminal_prim) // {
    PRIM("EMIT", x_emit)
END_DIRECT // }


META_FORTH(init_terminal_ops) // {
    XCOLON("CR") L('\n') EMIT XSEMICOLON
    XCOLON("SPACE") L(' ') EMIT XSEMICOLON
    XCOLON("TYPE") /* ( c-addr u -- ) */
	BEGIN DUP WHILE
	    TO_R DUP C_FETCH EMIT CHAR_PLUS R_FROM L(1) MINUS
	REPEAT DROP DROP
    XSEMICOLON
    XCOLON("SPACES") /* ( n -- ) */
	BEGIN DUP L(0) GREATER WHILE SPACE L(1) MINUS REPEAT
    XSEMICOLON

    XCOLON(".")
	LESS_NUMBER
	    L(' ') HOLD DUP ABS L(0) NUMBER_S ROT SIGN
	NUMBER_GREATER TYPE
    XSEMICOLON
    XCOLON("U.")
	LESS_NUMBER L(' ') HOLD L(0) NUMBER_S NUMBER_GREATER TYPE
    XSEMICOLON
END_META // }

// ."                    6.1.0190 CORE                   28
// ACCEPT                6.1.0695 CORE                   33
// KEY                   6.1.1750 CORE                   40
// SPACES                6.1.2230 CORE                   45
//
// .R                    6.2.0210 CORE EXT               49
// U.R                   6.2.2330 CORE EXT               57
