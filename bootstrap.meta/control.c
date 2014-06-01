/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <stddef.h>

#include "forth.h"

/*
 * control.c - Standard Forth words relating to flow of control in
 *   compiled defintions.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// +LOOP                 6.1.0140 CORE                   27
// BEGIN                 6.1.0760 CORE                   34
// DO                    6.1.1240 CORE                   36
// ELSE                  6.1.1310 CORE                   37
// I                     6.1.1680 CORE                   39
// IF                    6.1.1700 CORE                   40
// J                     6.1.1730 CORE                   40
// LEAVE                 6.1.1760 CORE                   41
// LOOP                  6.1.1800 CORE                   41
// REPEAT                6.1.2140 CORE                   44
// THEN                  6.1.2270 CORE                   46
// UNLOOP                6.1.2380 CORE                   47
// UNTIL                 6.1.2390 CORE                   47
// WHILE                 6.1.2430 CORE                   47
//
// ?DO                   6.2.0620 CORE EXT               51
// AGAIN                 6.2.0700 CORE EXT               51
// CASE                  6.2.0873 CORE EXT               52
// ENDCASE               6.2.1342 CORE EXT               53
// ENDOF                 6.2.1343 CORE EXT               53
// OF                    6.2.1950 CORE EXT               54
//------  ------  ------  ------  ------  ------  ------  ------


vminstr_p
do_skip(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    return ip + ip->offset;
}


vminstr_p
do_fskip(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    if (POP(vm) == 0)
	return ip + ip->offset;
    else
	return ip + 1;
}


META_FORTH(init_control_ops) // {
    XCOLON("BEGIN")	/* ( C: -- dest ) */
			HERE XSEMICOLON COMPILE_ONLY

    XCOLON("THEN")	/* ( C: orig -- ) */
	HERE OVER MINUS L(CELL_SHIFT) RSHIFT SWAP STORE
    XSEMICOLON COMPILE_ONLY

    /* : back ( C: dest -- ) HERE - , ; */
    XNONAME HERE MINUS L(CELL_SHIFT) RSHIFT COMMA XSEMICOLON
    /* : forward ( C: -- orig ) HERE 1 CELLS ALLOT ; */
    XNONAME HERE L(CELL_SIZE) ALLOT XSEMICOLON
    ANON(do_fskip)			/* ( C: back forward fskip ) */

    /* : IF ( C: -- orig ) POSTPONE fskip forward ; IMMEDIATE */
    XCOLON("IF")
	INTERP( DUP ) LITERAL COMMA INTERP( OVER COMMA )
    XSEMICOLON COMPILE_ONLY

    /* : UNTIL ( C: dest -- ) POSTPONE fskip back ; IMMEDIATE */
    XCOLON("UNTIL")
	INTERP( ROT SWAP )		/* ( C: forward back fskip ) */
	LITERAL COMMA INTERP( DUP COMMA )
    XSEMICOLON COMPILE_ONLY		/* ( C: forward back ) */

    ANON(do_skip)			/* ( C: forward back skip ) */

    /* : AGAIN ( C: dest -- ) POSTPONE skip back ; IMMEDIATE */
    XCOLON("AGAIN")
	INTERP( DUP ) LITERAL COMMA INTERP( SWAP COMMA )
    XSEMICOLON COMPILE_ONLY		/* ( C: forward skip ) */

    /* : ELSE ( C: orig1 -- orig2 )
	  POSTPONE skip forward SWAP POSTPONE THEN ; IMMEDIATE */
    XCOLON("ELSE")
	LITERAL COMMA INTERP( COMMA ) SWAP XPOSTPONE THEN
    XSEMICOLON COMPILE_ONLY

    XCOLON("WHILE")	/* ( C: dest -- orig dest ) */
	XPOSTPONE IF SWAP XSEMICOLON COMPILE_ONLY

    XCOLON("REPEAT")	/* ( C: orig dest -- ) */
	XPOSTPONE AGAIN XPOSTPONE THEN XSEMICOLON COMPILE_ONLY
END_META // }
