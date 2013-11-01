\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\ .                     6.1.0180 CORE                   27
\ ."                    6.1.0190 CORE                   28
\ ACCEPT                6.1.0695 CORE                   33
\ CR                    6.1.0990 CORE                   36
\ EMIT                  6.1.1320 CORE                   38
\ KEY                   6.1.1750 CORE                   40
\ SPACE                 6.1.2220 CORE                   45
\ SPACES                6.1.2230 CORE                   45
\ TYPE                  6.1.2310 CORE                   46
\ U.                    6.1.2320 CORE                   46
\
\ .R                    6.2.0210 CORE EXT               49
\ U.R                   6.2.2330 CORE EXT               57
\ ------  ------  ------  ------  ------  ------  ------  ------

\ EMIT ( char -- )
\ KEY ( -- char )

: CR ( -- ) 10 EMIT ;
: SPACE ( -- ) BL EMIT ;
: SPACES ( n -- ) 0 ?DO SPACE LOOP ;
: TYPE ( c-addr u -- )
    CHARS OVER + SWAP ?DO I C@ EMIT [ 1 CHARS ] LITERAL +LOOP
;
: ." POSTPONE S" TYPE ; IMMEDIATE

: . <# BL HOLD DUP ABS 0 #S ROT SIGN #> TYPE ;
: U. <# BL HOLD 0 #S #> TYPE ;

\ XXX - does this really work?  what about backspace, etc.?
: ACCEPT ( c-addr +n1 -- +n2 )
    DUP >R CHARS OVER + SWAP		( end start ) ( R: +n1 )
    BEGIN				( end cur ) ( R: +n1 )
	2DUP < WHILE
	    KEY DUP EMIT		( end cur char ) ( R: +n1 )
	DUP 10 <> WHILE
	    OVER C! CHAR+		( end cur ) ( R: +n1 )
    REPEAT THEN R> + - NEGATE
;
