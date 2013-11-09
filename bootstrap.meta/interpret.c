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


vminstr_p
i_lookup(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 2);
    cell_ft *sp = SP(vm);
    char * id = (char *) PICK(sp, 1);
    cell_ft len = PICK(sp, 0);
    name_p name = lookup(id, len);
    if (name == NULL) {
	PICK(sp, 1) = F_FALSE;
	SET_SP(vm, sp, 1);
    } else {
	CHECK_PUSH(vm, 1);
	SET_SP(vm, sp, -1);
	PICK(sp, 1) = (cell_ft) NAME_XT(name);
	PICK(sp, 0) = (cell_ft) NAME_FLAGS(name);
	PICK(sp, -1) = F_TRUE;
    }
    return ip;
}


vminstr_p
i_throw(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    cell_ft exc = POP(vm);
    if (exc != 0) {
	THROW(vm, exc);
    }
    return ip;
}


META_FORTH(init_interpreter_ops) // {
    L(i_throw) L(DO_THROW) STORE
    L(i_lookup) L(DO_LOOKUP) STORE

    XNONAME					// ( C: parse-word )
	SOURCE SWAP TO_IN FETCH CHARS PLUS TO_R		// ( R: c-addr )
	TO_IN FETCH MINUS L(-1)
	BEGIN ONE_PLUS TWO_DUP GREATER WHILE		// ( end idx )
	    DUP CHARS R_FETCH SWAP PLUS C_FETCH		// ( end idx c )
	    L(' ' + 1) MINUS L('~' - ' ') U_LESS
	UNTIL THEN TO_IN PLUS_STORE DROP R_FROM DROP
	L(' ') PARSE
    XSEMICOLON

    XNONAME					// ( C: parse-word interpret )
    BEGIN INTERP( ROT COMMA ) DUP WHILE		// ( str len )
	TWO_DUP INTERP( L(DO_LOOKUP) COMMA ) IF		// ( str len xt flags )
	    // compile or execute a definition
	    TWO_SWAP TWO_DROP STATE FETCH IF
		NF_IMMEDIATE AND IF EXECUTE ELSE COMMA THEN
	    ELSE
		NF_NO_INTERPRET AND IF
		    L(-14) INTERP( L(DO_THROW) COMMA )
		ELSE
		    EXECUTE
		THEN
	    THEN
	ELSE					// ( str len )
	    // try to convert a number
	    OVER C_FETCH L('-') EQUALS TO_R	// ( R: neg? )
	    // adjust for sign char
	    R_FETCH PLUS SWAP R_FETCH CHARS MINUS SWAP
	    L(0) DUP TWO_SWAP TO_NUMBER		// ( ul uh str len )
	    IF L(-13) INTERP( L(DO_THROW) COMMA ) THEN TWO_DROP	// ( ul )
	    R_FROM IF NEGATE THEN				// ( n )
	    STATE FETCH IF XPOSTPONE LITERAL THEN
	THEN
    REPEAT TWO_DROP XSEMICOLON

    XVARIABLE("INTERPRET")
    DUP S("INTERPRET") STORE

					// ( C: interpret )
    XCOLON("EVALUATE")			// ( i*x c-addr u -- j*x )
	// save current source specification
	INTERP( TO_IN ) LITERAL DUP TWO_FETCH TWO_TO_R
	DUP INTERP( L(2) CELLS ) LITERAL PLUS TWO_FETCH TWO_TO_R
	// make the string the input source
	TO_R L(0) R_FETCH STORE
	L(-1) R_FETCH CELL_PLUS STORE
	R_FROM INTERP( L(2) CELLS ) LITERAL PLUS TWO_STORE
	INTERP( DUP COMMA )
	// restore original source specification
	TWO_R_FROM TWO_R_FROM		// ( src id >in )
	INTERP( TO_IN ) LITERAL TWO_STORE
	INTERP( TO_IN L(2) CELLS PLUS ) LITERAL TWO_STORE
    XSEMICOLON

					// ( C: interpret )
    ANON(do_clearrstack)		// ( C: interpret clearrstack )
    XCOLON("QUIT")
	INTERP( COMMA )			// ( C: interpret )
	L(0) INTERP( TO_IN CELL_PLUS ) LITERAL STORE
	XPOSTPONE LEFT_BRACKET
	BEGIN				// ( C: interpret dest )
	    STATE FETCH L(0) EQUALS IF
		// XXX implement ."
		L('o') EMIT L('k') EMIT L(' ') EMIT
	    THEN
	    REFILL WHILE		// ( C: interpret orig dest )
		    INTERP( ROT COMMA )
	REPEAT
	L('b') EMIT L('y') EMIT L('e') EMIT CR
	L(0) TO_R L(0) TO_R
    XSEMICOLON
END_META // }
