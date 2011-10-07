VARIABLE STATE
: [ FALSE STATE ! ; IMMEDIATE
: ] TRUE  STATE ! ;
[

: error ." Error: " . ABORT ;
\ require-interpret - signal an error if not in interpretation state
\ require-compile   - signal an error if not in compilation state
\                     or there is no current definition
: require-interpret ( -- ) STATE @    IF -29 error THEN ;
: require-compile   ( -- ) STATE @ 0= IF -14 error THEN ;

HERE VALUE curdef
HERE VALUE lastdef

: mcp, ( mcp -- ) ALIGN , ;

\ startdef - start a new definition in the dictionary; not yet findable
: startdef ( mcp -- )
    ALIGN HERE TO curdef lastdef , 0 ,
    parse-name DUP C@ CHARS + DO I C@ C, [ 1 CELLS ] LITERAL +LOOP
    mcp,
;

\ enddef - finish a definition; make it findable
: enddef ( -- ) curdef TO lastdef ALIGN ;

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
