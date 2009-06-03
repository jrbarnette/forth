: skip-chars ( char -- )
    source over >in @ chars + >r >in @ do
	dup i chars +
;

: PARSE ( c "ccc<c>" -- c-addr len )
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

: WORD ( char “<chars>ccc<char>” -- c-addr )
    dup skip-chars parse		( src nchars )
    here aligned cell+ dup >r		( src nchars dst ) ( R: dst )
    \ counted string length
    2dup c! char+ swap chars		( src dst nbytes ) ( R: dst )
    \ copy data then append blank
    2dup + >r move bl r> c! r>		( dst )
;
