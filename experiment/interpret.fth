32 CONSTANT BL
: NIP SWAP DROP ;
: 1+ 1 + ;
: 0= 0 = ;

: PARSE ( c "ccc<c>" -- c-addr len )
    SOURCE SWAP >IN @ CHARS + SWAP >IN @ - 0 ?DO	( c start )
	2OVER I CHARS + C@ = IF			( c start )
	    NIP I DUP 1+ >IN +! UNLOOP EXIT
	THEN
    LOOP					( c start )
    NIP SOURCE NIP DUP >IN @ - SWAP >IN !
;

: skip ( c "ccc" -- )
    SOURCE SWAP >IN @ CHARS + SWAP >IN @ - 0 ?DO	( c start )
	2OVER I CHARS + C@ <> IF		( c start )
	    2DROP I >IN +! UNLOOP EXIT
	THEN
    LOOP					( c start )
    2DROP SOURCE >IN ! DROP
;

: parse-name ( "<spaces>name" -- addr len )
    BL skip BL PARSE
    DUP 0=   IF -16 THROW THEN
    DUP 31 > IF -19 THROW THEN
;

\ STATE is FALSE in interpretation state
\ STATE is TRUE in compilation state
\ FIND flag is 1 for IMMEDIATE words
\ FIND flag is -1 when not IMMEDIATE
: interpret
    BEGIN parse-name ?DUP WHILE		( name len )
	sfind ?DUP IF			( xt 1 | xt -1 )
	    \ EXECUTE when immediate or not compiling
	    \ COMPILE, when not immediate and compiling
	    -1 <> STATE @ 0= OR IF EXECUTE ELSE COMPILE, THEN
	ELSE				( name len )
	    0 0 2SWAP >NUMBER IF	( lo hi addr )
		-13 THROW
	    ELSE			( lo hi addr )
		DROP IF
		    \ unspecified condition - number overflow
		THEN
		STATE @ IF POSTPONE LITERAL THEN
	    THEN
	THEN
    REPEAT DROP
;

: EVALUATE ( i*x addr len -- j*x ) -1 set-source interpret ;

: QUIT
    clearrsp 0 set-source POSTPONE [
    BEGIN REFILL WHILE
	interpret STATE @ 0= IF CR ." OK " THEN
    REPEAT BYE
;

: ABORT clearsp QUIT ;
