/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include "forth.h"

/*
 * interpret.c - Standard Forth definitions relating to the text
 * interpreter.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// ABORT                 6.1.0670 CORE                   32
// ABORT"                6.1.0680 CORE                   32
// EVALUATE              6.1.1360 CORE                   38
// QUIT                  6.1.2050 CORE                   43
//------  ------  ------  ------  ------  ------  ------  ------

/* ( R: i*x -- ) */
vminstr_p
do_clearrstack(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CLEAR_RSTACK(vm);
    return ip;
}


META_FORTH(interpreter_ops) // {
    XNONAME BEGIN parse-name DUP WHILE		/* ( str len ) */
	TWO_DUP lookup IF			/* ( str len xt flags ) */
	    /* compile or execute a definition */
	    TWO_SWAP TWO_DROP STATE FETCH IF
		NF_IMMEDIATE AND IF EXECUTE ELSE COMMA THEN
	    ELSE
		NF_IMMEDIATE AND IF L(-14) throw ELSE EXECUTE THEN
	    THEN
	ELSE					/* ( str len ) */
	    /* try to convert a number */
	    OVER C_FETCH L('-') EQUALS TO_R	/* ( R: neg? ) */
	    /* adjust for sign char */
	    R_FETCH PLUS SWAP R_FETCH MINUS SWAP
	    L(0) DUP TWO_SWAP TO_NUMBER		/* ( ul uh str len ) */
	    IF L(-13) throw THEN TWO_DROP	/* ( ul ) */
	    R_FROM IF NEGATE THEN		/* ( n ) */
	    STATE FETCH IF XPOSTPONE LITERAL THEN
	THEN
    REPEAT XSEMICOLON

					/* ( C: interpret ) */
    ANONYMOUS(do_clearrstack)		/* ( C: interpret clearrstack ) */

    XCOLON("QUIT")
	INTERP( COMMA )			/* ( C: interpret ) */
	XPOSTPONE LEFT_BRACKET
	BEGIN
	    STATE FETCH L(0) EQUALS IF CR
	    /* ." OK " */
	    THEN
	    REFILL WHILE
	    LEFT_BRACKET COMMA RIGHT_BRACKET
	    /* ( C: ) */
	REPEAT
    XSEMICOLON
END_META // }
