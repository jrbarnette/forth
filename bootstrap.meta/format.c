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

    /* 1 CELLS 8 * 2 * 2 + ALLOT HERE CONSTANT hold-end */
    /* VARIABLE hold-count */
    L(2 * 8 * CELL_SIZE + 2) ALLOT HERE L(CELL_SIZE) ALLOT

    /* : hold-pointer hold-end hold-count @ CHARS - ; */
    XNONAME /* ( C: count-ptr xt ) */
	INTERP( OVER DUP ) LITERAL LITERAL FETCH CHARS MINUS
    XSEMICOLON
    
    /* ( C: count-ptr hold-ptr-xt ) */

    /* : <# ( -- ) 0 hold-count ! ; */
    XCOLON("<#") L(0) INTERP( OVER ) LITERAL STORE XSEMICOLON
    /* : HOLD ( char -- ) 1 hold-count +! hold-pointer c! ; */
    XCOLON("HOLD")
	L(1) INTERP( OVER ) LITERAL PLUS_STORE
	INTERP( DUP COMMA ) C_STORE
    XSEMICOLON
    /* : #> ( d -- c-addr u ) 2DROP hold-pointer hold-count @ ; */
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
	// ( ud1 c-addr1 u1 -- ud2 c-addr2 u2 )
	BEGIN DUP WHILE
	    // ( ud c-addr len )
	    TO_R DUP TO_R C_FETCH L('0') MINUS
	    // ( ud digit ) ( R: len c-addr )
	    L(9) OVER U_LESS IF L('0' - 'A') PLUS THEN
	    // ( ud digit ) ( R: len c-addr )
	    DUP BASE FETCH U_LESS INVERT
	    // ( ud digit flag ) ( R: len c-addr )
		IF DROP R_FROM R_FROM EXIT THEN
	    // ( ud digit ) ( R: len c-addr )
	    TO_R BASE FETCH STAR TO_R BASE FETCH UM_STAR R_FROM PLUS
	    // ( ul*base uh*base ) ( R: len c-addr digit )
	    R_FROM ROT DUP TO_R PLUS DUP R_FROM U_LESS NEGATE ROT PLUS
	    // ( ul*base+digit uh*base+carry ) ( R: len c-addr )
	    R_FROM CHAR_PLUS R_FROM L(1) MINUS
	    // ( ud c-addr len )
	REPEAT
    XSEMICOLON

    XCOLON("HEX") L(16) BASE STORE XSEMICOLON
END_META // }
