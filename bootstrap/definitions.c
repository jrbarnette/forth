/*
 * Copyright 2007, by J. Richard Barnette
 */

/*
    :                 6.1.0450 CORE                   30
    ;                 6.1.0460 CORE                   30
    >BODY             6.1.0550 CORE                   31
    CONSTANT          6.1.0950 CORE                   35
    CREATE            6.1.1000 CORE                   36
    DOES>             6.1.1250 CORE                   37
    EXIT              6.1.1380 CORE                   39
    FIND              6.1.1550 CORE                   39
    IMMEDIATE         6.1.1710 CORE                   41
    POSTPONE          6.1.2033 CORE                   43
    RECURSE           6.1.2120 CORE                   44
    VARIABLE          6.1.2410 CORE                   48
    :NONAME           6.2.0455 CORE EXT               52
    COMPILE,          6.2.0945 CORE EXT               54
    MARKER            6.2.1850 CORE EXT               56
    TO                6.2.2295 CORE EXT               59
    VALUE             6.2.2405 CORE EXT               60
    [COMPILE]         6.2.2530 CORE EXT               60
*/

/* -------------------------------------------------------------- */
void
init_definers()
{
    /* '                 6.1.0070 CORE */
    /* : ' BL WORD FIND 0= IF DROP -32 THROW THEN ; */
    COLON(tick);
	LIT(' '); XT(word); XT(find); XT(zero_equal); IF();
	    LIT(-32); XT(throw);
	THEN();
    SEMICOLON();
}
