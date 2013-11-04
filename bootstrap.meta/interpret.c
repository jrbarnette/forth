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
    c_addr_ft id = (c_addr_ft) PICK(sp, 1);
    cell_ft len = (c_addr_ft) PICK(sp, 0);
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


META_FORTH(interpreter_ops) // {
    ANON(i_lookup)					// ( C: lookup )
    XNONAME						// ( C: lookup word )
	// ( "<spaces>name" -- c-addr u )
	SOURCE SWAP TO_R				// ( R: char c-addr )
	TO_IN FETCH ONE_MINUS
	BEGIN ONE_PLUS TWO_DUP GREATER WHILE		// ( end idx )
	    DUP CHARS TWO_R_FETCH ROT PLUS C_FETCH	// ( end idx char c )
	    delimiter? invert
	UNTIL THEN TO_IN STORE DROP TWO_R_FETCH TWO_DROP
	L(' ') PARSE
    XSEMICOLON
    XNONAME					// ( C: lookup word interp )
    BEGIN INTERP( SWAP COMMA ) DUP WHILE	// ( str len )
	TWO_DUP INTERP( SWAP COMMA ) IF		// ( str len xt flags )
	    /* compile or execute a definition */
	    TWO_SWAP TWO_DROP STATE FETCH IF
		NF_IMMEDIATE AND IF EXECUTE ELSE COMMA THEN
	    ELSE
		NF_IMMEDIATE AND IF L(-14) throw ELSE EXECUTE THEN
	    THEN
	ELSE					// ( str len )
	    /* try to convert a number */
	    OVER C_FETCH L('-') EQUALS TO_R	// ( R: neg? )
	    /* adjust for sign char */
	    R_FETCH PLUS SWAP R_FETCH MINUS SWAP
	    L(0) DUP TWO_SWAP TO_NUMBER		// ( ul uh str len )
	    IF L(-13) throw THEN TWO_DROP	// ( ul )
	    R_FROM IF NEGATE THEN		// ( n )
	    STATE FETCH IF XPOSTPONE LITERAL THEN
	THEN
    REPEAT XSEMICOLON

					// ( C: interpret )
    ANON(do_clearrstack)		// ( C: interpret clearrstack )

    XCOLON("QUIT")
	INTERP( COMMA )			// ( C: interpret )
	// FIXME set source to terminal
	XPOSTPONE LEFT_BRACKET
	BEGIN
	    STATE FETCH L(0) EQUALS IF
		// XXX implement ."
		CR L('O') EMIT L('K') EMIT L(' ') EMIT
	    THEN
	    REFILL WHILE INTERP( COMMA )	// ( C: )
	REPEAT
    XSEMICOLON
END_META // }
