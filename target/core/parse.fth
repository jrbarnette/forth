\ Copyright 2011, by J. Richard Barnette
\ ------  ------  ------  ------  ------  ------  ------

\ (                     6.1.0080 CORE                   26
\ >IN                   6.1.0560 CORE                   31
\ SOURCE                6.1.2216 CORE                   45
\ WORD                  6.1.2450 CORE                   48
\ .(                    6.2.0200 CORE EXT               49
\ PARSE                 6.2.2008 CORE EXT               55
\ REFILL                6.2.2125 CORE EXT               55
\ RESTORE-INPUT         6.2.2148 CORE EXT               56
\ SAVE-INPUT            6.2.2182 CORE EXT               56
\ SOURCE-ID             6.2.2218 CORE EXT               56
\ \                     6.2.2535 CORE EXT               58

\ ------  ------  ------  ------  ------  ------  ------
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
