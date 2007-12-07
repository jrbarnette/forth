: PARSE ( c "ccc<c>" -- c-addr len )
    >R						( R: c )
    SOURCE CHARS OVER + SWAP >IN @ CHARS +	( end c-addr )
    DUP 0 2SWAP ?DO				( c-addr len )
	1+ I C@ R@ = IF DUP >IN +! LEAVE THEN
    [ 1 CHARS ] LITERAL +LOOP
    R> DROP
;

: parse-word ( "<bl>ccc<bl>" -- c-addr len | 0 )
    SOURCE CHARS OVER + SWAP >IN @ CHARS + ?DO
	I C@ BL <> IF LEAVE THEN
    1+ [ 1 CHARS ] LITERAL +LOOP
    >IN ! BL PARSE ?DUP IF DROP 0 THEN
;

: parse-name ( "<spaces>name" -- c-addr )
    BL WORD DUP C@			( addr len )
    ?DUP 0= IF DROP -16 error THEN
       31 > IF DROP -19 error THEN
;
