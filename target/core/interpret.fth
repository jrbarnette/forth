\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\ ABORT                 6.1.0670 CORE                   32
\ ABORT"                6.1.0680 CORE                   32
\ EVALUATE              6.1.1360 CORE                   38
\ QUIT                  6.1.2050 CORE                   43
\ STATE                 6.1.2250 CORE                   45
\ [                     6.1.2500 CORE                   48
\ ]                     6.1.2540 CORE                   49
\ ------  ------  ------  ------  ------  ------  ------  ------

variable STATE
: [ false state ! ; immediate
: ] true  state ! ;

\ FIXME - circular definition error -> ABORT -> QUIT -> interpret -> error

: error ." Error: " . ABORT ;
\ require-interpret - signal an error if not in interpretation state
\ require-compile   - signal an error if not in compilation state
\                     or there is no current definition
: require-interpret ( -- ) STATE @    IF -29 error THEN ;
: require-compile   ( -- ) STATE @ 0= IF -14 error THEN ;

: interpret
    BEGIN
    BL WORD DUP C@ WHILE	( c-addr )
	FIND ?DUP IF		( xt -1 | xt 1 ) \ 1 means IMMEDIATE
	    -1 = STATE @ AND IF COMPILE, ELSE EXECUTE THEN
	ELSE			( c-addr )
	    0 0 ROT		( ud c-addr )
	    COUNT OVER		( ud c-addr u c-addr )
	    C@ [CHAR] - = IF 1- SWAP 1+ SWAP -1 ELSE 0 THEN >R
	    >NUMBER 2SWAP SWAP DROP R> IF NEGATE THEN
	    SWAP IF -13 error THEN
	    SWAP DROP
	    STATE @ IF POSTPONE LITERAL THEN
	THEN
    REPEAT
;

: QUIT
    clearrsp
    \ XXX 0 TO SOURCE-ID
    POSTPONE [
    BEGIN REFILL WHILE
	interpret STATE @ 0= IF CR ." OK " THEN
    REPEAT BYE
;

: ABORT clearsp QUIT ;
\ : ABORT" ... ;
