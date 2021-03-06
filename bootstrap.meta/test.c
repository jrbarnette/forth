/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include "forth.h"

/*
 * test.c - Sanity tests for use in initialization.
 */


META_FORTH(dump_dictionary) // {
#if 0
    XCOLON("COUNT") DUP CHAR_PLUS SWAP C_FETCH XSEMICOLON
    XCOLON("ADDR.")
	/* ( addr -- ) */
	BASE FETCH SWAP HEX
	L(0) LESS_NUMBER L(2 * CELL_SIZE) BEGIN
	    TO_R NUMBER_SIGN R_FROM
	    L(1) MINUS DUP L(0) EQUALS
	UNTIL DROP NUMBER_GREATER TYPE
	BASE STORE
    XSEMICOLON
    XCOLON("DUMP-WORDLIST")
	/* ( wid -- ) */
	BASE FETCH SWAP HEX
	BEGIN FETCH DUP WHILE
	    DUP
		S("ADDR.")
	    DUP CELL_PLUS COUNT DUP L(~0x1f) AND TO_R L(0x1f) AND
	    OVER OVER CHARS PLUS ALIGNED FETCH
		SPACE SPACE S("ADDR.")
	    R_FROM L(0) LESS_NUMBER NUMBER_SIGN NUMBER_SIGN NUMBER_GREATER
		SPACE TYPE
		SPACE TYPE CR
	REPEAT DROP
	BASE STORE
    XSEMICOLON
    XCOLON("DUMP-DICTIONARY")
	HERE S("ADDR.") CR
	L(&DICT.forth_wordlist) S("DUMP-WORDLIST")
	L(DICT_START) S("ADDR.") CR
	L(DICT_END) L(DICT_START) MINUS DOT
	HERE L(DICT_START) MINUS DOT
	L(DICT_END) HERE MINUS DOT CR
    XSEMICOLON
#endif
END_META // }


META_FORTH(test_forth) // {
    // ( x* -- )
    // stack is expected empty; emit '!' for every excess element
    XCOLON("CHECKSTACK")
	S("DEPTH") BEGIN DUP WHILE
	    SWAP DROP L('!') EMIT L(1) MINUS
	REPEAT DROP
    XSEMICOLON

    // ( flag -- )
    // flag indicates pass or file; emit 'P' or 'F' to indicate
    XCOLON("REPORT")
	IF L('P') ELSE L('F') THEN EMIT
    XSEMICOLON

    // ( x -- )
    // pass iff x is a true value
    // require x be the only element on the stack
    XCOLON("ASSERT-TRUE")
	S("REPORT") S("CHECKSTACK")
    XSEMICOLON

    // ( x -- )
    // pass iff x is a false value
    // require x be the only element on the stack
    XCOLON("ASSERT-FALSE")
	L(0) EQUALS S("REPORT") S("CHECKSTACK")
    XSEMICOLON

    // sanity checks; if these can't pass, nothing is reliable
    L(0) S("CHECKSTACK") S("CHECKSTACK")
    L(F_TRUE) S("ASSERT-TRUE")
    L(F_FALSE) S("ASSERT-FALSE")

    XCOLON("TEST-IF") IF L(F_TRUE) THEN XSEMICOLON
    L(F_TRUE) S("TEST-IF") S("ASSERT-TRUE")
    L(1) S("TEST-IF") S("ASSERT-TRUE")
    L(F_FALSE) S("TEST-IF") S("CHECKSTACK")

    XCOLON("TEST-ELSE") IF L(F_FALSE) ELSE L(F_TRUE) THEN XSEMICOLON
    L(F_TRUE) S("TEST-ELSE") S("ASSERT-FALSE")
    L(F_FALSE) S("TEST-ELSE") S("ASSERT-TRUE")
    CR

    // XXX arith prim tests
    // XXX stack prim tests
    // XXX memory prim tests
    // XXX mult prim tests
    // compile prim tests - everywhere you look
    // terminal prim tests - can't automate

    // memory ops tests
    L('@') EMIT
    L(1) CELLS L(0) CELL_PLUS EQUALS S("ASSERT-TRUE")
    L(1) CELLS L(1) ALIGNED EQUALS S("ASSERT-TRUE")
    L(1) CHARS L(0) CHAR_PLUS EQUALS S("ASSERT-TRUE")
    // XXX test +!
    CR

    // dictionary ops tests
    L(',') EMIT
    HERE HERE ALIGNED EQUALS S("ASSERT-TRUE")
    HERE L(1) ALLOT L(-1) ALLOT HERE EQUALS S("ASSERT-TRUE")
    XCOLON("TEST-ALLOT")
	/* ( here count -- ) */
	OVER TO_R PLUS HERE EQUALS R_FROM HERE MINUS ALLOT
	S("ASSERT-TRUE")
    XSEMICOLON
    HERE L(1) CELLS DUP ALLOT S("TEST-ALLOT")
    HERE L(1) CHARS DUP ALLOT S("TEST-ALLOT")
    HERE L(1) ALLOT ALIGN L(1) CELLS S("TEST-ALLOT")
    HERE L(3) COMMA L(1) CELLS S("TEST-ALLOT")
    L(3) HERE FETCH EQUALS S("ASSERT-TRUE")
    HERE L('A') C_COMMA L(1) CHARS S("TEST-ALLOT")
    L('A') HERE C_FETCH EQUALS S("ASSERT-TRUE")
    CR

    // compile ops tests - everywhere you look
    // control ops tests - mostly covered

    // arith ops
    L('+') EMIT
    L(-1) ABS L(1) EQUALS S("ASSERT-TRUE")
    L(1) ABS L(1) EQUALS S("ASSERT-TRUE")
    L(0) ABS S("ASSERT-FALSE")
    CR

    // format ops
    L('#') EMIT
    L(2) BASE STORE DECIMAL BASE FETCH L(10) EQUALS S("ASSERT-TRUE")
    HEX BASE FETCH L(16) EQUALS S("ASSERT-TRUE")
    DECIMAL

    // ( char ... c-addr u -- )
    // check that ( c-addr u ) points to the characters on the stack.
    // results in the form of <????>, ? is 'P' or 'F' for each
    // character, in order.
    XCOLON("CHECK-HOLD-STRING")
	S("DEPTH") L(2) MINUS EQUALS DUP S("REPORT")
	L('<') EMIT IF
	    TO_R BEGIN S("DEPTH") WHILE
		R_FROM DUP CHAR_PLUS TO_R
		C_FETCH EQUALS S("REPORT")
	    REPEAT R_FROM
	THEN DROP S("CHECKSTACK") L('>') EMIT
    XSEMICOLON

    LESS_NUMBER S("CHECKSTACK") L(0) L(0) NUMBER_GREATER
	S("CHECK-HOLD-STRING")

    L('A') LESS_NUMBER DUP HOLD L(0) L(0) NUMBER_GREATER
	S("CHECK-HOLD-STRING")

    XCOLON("TEST-SIGN")
	LESS_NUMBER SIGN L(0) L(0) NUMBER_GREATER
	S("CHECK-HOLD-STRING")
    XSEMICOLON
    L(1) S("TEST-SIGN")
    L(0) S("TEST-SIGN")
    L('-') L(-1) S("TEST-SIGN")

    XCOLON("CHECK-#")
	LESS_NUMBER NUMBER_SIGN NUMBER_GREATER
	S("CHECK-HOLD-STRING")
    XSEMICOLON

    XCOLON("CHECK-BASE-#")
	BASE FETCH TO_R BASE STORE CR L('#') EMIT L('(') EMIT
	// single digit test
	L('0') L(0) L(0) S("CHECK-#")
	L('1') L(1) L(0) S("CHECK-#")
	BASE FETCH L(1) MINUS DUP DUP
	    L(10) LESS IF L('0') ELSE L('A'-10) THEN PLUS
	SWAP L(0) S("CHECK-#")
	// two digit test
	L('0') BASE FETCH L(0) S("CHECK-#")
	L('1') BASE FETCH L(1) PLUS L(0) S("CHECK-#")
	R_FROM BASE STORE
	L(')') EMIT
    XSEMICOLON
    L(2) S("CHECK-BASE-#")
    L(10) S("CHECK-BASE-#")
    L(36) S("CHECK-BASE-#")

    // 0
    // 1
    // BASE-1
    // BASE
    // BASE+1
    XCOLON("CHECK-#S")
	LESS_NUMBER NUMBER_S NUMBER_GREATER
	S("CHECK-HOLD-STRING")
    XSEMICOLON

    XCOLON("CHECK-BASE-#S")
	BASE FETCH TO_R BASE STORE CR L('#') EMIT L('(') EMIT
	// single digit test
	L('0') L(0) L(0) S("CHECK-#S")
	L('1') L(1) L(0) S("CHECK-#S")
	BASE FETCH L(1) MINUS DUP DUP
	    L(10) LESS IF L('0') ELSE L('A'-10) THEN PLUS
	SWAP L(0) S("CHECK-#S")
	// two digit test
	L('0') L('1') BASE FETCH L(0) S("CHECK-#S")
	L('1') L('1') BASE FETCH L(1) PLUS L(0) S("CHECK-#S")
	R_FROM BASE STORE
	L(')') EMIT
    XSEMICOLON
    L(2) S("CHECK-BASE-#S")
    L(10) S("CHECK-BASE-#S")
    L(36) S("CHECK-BASE-#S")
    CR

    // parse ops
    L('p') EMIT
    XCOLON("SOURCE!")	// ( c-addr u -- )
	L(0) TO_IN STORE
	L(-1) TO_IN CELL_PLUS STORE
	TO_IN L(2) CELLS PLUS TWO_STORE
    XSEMICOLON
    L("") L(0) S("SOURCE!")
    SOURCE SWAP DROP L(0) EQUALS S("ASSERT-TRUE")
    L(' ') PARSE L(0) EQUALS S("REPORT")
    SOURCE DROP EQUALS S("ASSERT-TRUE")
    TO_IN FETCH L(0) EQUALS S("ASSERT-TRUE")
    SPACE

    L("ABA") L(3) S("SOURCE!")
    L('B') PARSE L(1) EQUALS S("REPORT")
    SOURCE DROP EQUALS S("ASSERT-TRUE")
    TO_IN FETCH L(2) EQUALS S("ASSERT-TRUE")
    L('B') PARSE L(1) EQUALS S("REPORT")
    SOURCE ONE_MINUS CHARS PLUS EQUALS S("ASSERT-TRUE")
    TO_IN FETCH L(3) EQUALS S("ASSERT-TRUE")
    SPACE

    L("A") L(1) S("SOURCE!")
    L('B') PARSE L(1) EQUALS S("REPORT")
    SOURCE DROP EQUALS S("ASSERT-TRUE")
    TO_IN FETCH L(1) EQUALS S("ASSERT-TRUE")
    SPACE

    L("A A") L(3) S("SOURCE!")
    L(' ') PARSE L(1) EQUALS S("REPORT")
    SOURCE DROP EQUALS S("ASSERT-TRUE")
    TO_IN FETCH L(2) EQUALS S("ASSERT-TRUE")
    SPACE

    L("A\nA") L(3) S("SOURCE!")
    L(' ') PARSE L(1) EQUALS S("REPORT")
    SOURCE DROP EQUALS S("ASSERT-TRUE")
    TO_IN FETCH L(2) EQUALS S("ASSERT-TRUE")
    SPACE

    L("A\0177A") L(3) S("SOURCE!")
    L(' ') PARSE L(1) EQUALS S("REPORT")
    SOURCE DROP EQUALS S("ASSERT-TRUE")
    TO_IN FETCH L(2) EQUALS S("ASSERT-TRUE")
    SPACE

    L("A~A") L(3) S("SOURCE!")
    L(' ') PARSE L(3) EQUALS S("REPORT")
    SOURCE DROP EQUALS S("ASSERT-TRUE")
    TO_IN FETCH L(3) EQUALS S("ASSERT-TRUE")
    SPACE

    L("A!A") L(3) S("SOURCE!")
    L(' ') PARSE L(3) EQUALS S("REPORT")
    SOURCE DROP EQUALS S("ASSERT-TRUE")
    TO_IN FETCH L(3) EQUALS S("ASSERT-TRUE")
    SPACE

    L(" A ") L(3) S("SOURCE!")
    L(' ') S("PARSE-WORD") L(1) EQUALS S("REPORT")
    SOURCE DROP CHAR_PLUS EQUALS S("ASSERT-TRUE")
    TO_IN FETCH L(3) EQUALS S("ASSERT-TRUE")
    CR

    L('X') EMIT
    L("+") L(1) L(DO_LOOKUP) EXECUTE ROT DROP S("REPORT")
    L(0) EQUALS S("ASSERT-TRUE")
    SPACE

    L("+") L(1) S("SOURCE!") L(1) L(2)
    S("INTERPRET") FETCH EXECUTE L(3) EQUALS S("ASSERT-TRUE")

    DECIMAL L("1") L(1) S("SOURCE!")
    S("INTERPRET") FETCH EXECUTE L(1) EQUALS S("ASSERT-TRUE")

    // L(1) L(2) L("+") L(1) EVALUATE
    // L(3) EQUALS S("ASSERT-TRUE")
    CR

    // CALL(dump_dictionary)
END_META // }
