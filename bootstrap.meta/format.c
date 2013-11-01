/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include "forth.h"

/*
 * format.c - Standard Forth words for pictured string formatting.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// #                     6.1.0030 CORE                   25
// #>                    6.1.0040 CORE                   25
// #S                    6.1.0050 CORE                   25
// <#                    6.1.0490 CORE                   31
// >NUMBER               6.1.0570 CORE                   31
// BASE                  6.1.0750 CORE                   34
// DECIMAL               6.1.1170 CORE                   36
// HOLD                  6.1.1670 CORE                   39
// SIGN                  6.1.2210 CORE                   45
//
// HEX                   6.2.1660 CORE EXT               54
//------  ------  ------  ------  ------  ------  ------  ------

META_FORTH(init_format_ops) // {
    XVARIABLE("BASE")
    XCOLON("DECIMAL") L(10) BASE STORE XSEMICOLON
    DECIMAL

    L(2 * 8 * CELL_SIZE + 2) ALLOT HERE L(CELL_SIZE) ALLOT

    XNONAME INTERP( DUP DUP ) LITERAL LITERAL FETCH CHARS MINUS
    XSEMICOLON
    
    /* ( C: count-ptr hold-ptr-xt ) */

    XCOLON("<#") L(0) INTERP( OVER ) LITERAL STORE XSEMICOLON
    XCOLON("HOLD")
	L(1) INTERP( OVER ) LITERAL PLUS_STORE
	INTERP( DUP COMMA ) C_STORE
    XSEMICOLON
    XCOLON("#>")
	DROP DROP INTERP( COMMA ) LITERAL FETCH
    XSEMICOLON
    
    /* ( C: ) */

    XCOLON("SIGN") L(0) LESS IF L('-') HOLD THEN XSEMICOLON
    XCOLON("#")
	L(0) BASE FETCH UM_SLASH_MOD ROT SWAP BASE FETCH UM_SLASH_MOD
	DUP L(10) U_LESS IF L('0') ELSE L('A' - 10) THEN
	PLUS HOLD SWAP
    XSEMICOLON
    XCOLON("#S")
	BEGIN NUMBER_SIGN OVER OVER OR L(0) EQUALS UNTIL
    XSEMICOLON
    XCOLON(">NUMBER")
	BEGIN DUP WHILE
	    TO_R DUP TO_R C_FETCH L('0') MINUS
	    L(9) OVER U_LESS IF L('0' - 'A') PLUS THEN
	    DUP BASE FETCH U_LESS INVERT
		IF DROP R_FROM R_FROM EXIT THEN
	    TO_R BASE FETCH STAR TO_R BASE FETCH UM_STAR R_FROM PLUS
	    R_FROM ROT DUP TO_R PLUS DUP R_FROM U_LESS NEGATE ROT PLUS
	    R_FROM CHAR_PLUS R_FROM L(1) MINUS
	REPEAT
    XSEMICOLON

    XCOLON("HEX") L(16) BASE STORE XSEMICOLON
END_META // }
