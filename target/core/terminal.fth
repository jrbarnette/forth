: CR 10 EMIT ;
: SPACE 20 EMIT ;
: SPACES 0 ?DO SPACE LOOP ;
: TYPE ( c-addr u -- )
    CHARS OVER + SWAP ?DO I C@ EMIT [ 1 CHARS ] LITERAL +LOOP
;

: ACCEPT ( c-addr +n1 -- +n2 )
    DUP >R CHARS OVER + SWAP		( end start ) ( R: +n1 )
    BEGIN				( end cur ) ( R: +n1 )
	2DUP < WHILE
	    KEY DUP EMIT		( end cur char ) ( R: +n1 )
	DUP 10 <> WHILE
	    OVER C! CHAR+		( end cur ) ( R: +n1 )
    REPEAT THEN R> + - NEGATE
;
