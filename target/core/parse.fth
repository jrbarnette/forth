\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\ (                     6.1.0080 CORE                   26
\ >IN                   6.1.0560 CORE                   31
\ CHAR                  6.1.0895 CORE                   35
\ SOURCE                6.1.2216 CORE                   45
\ WORD                  6.1.2450 CORE                   48
\ ------  ------  ------  ------  ------  ------  ------  ------

create SOURCE 2 cells allot
    here ] 2@ exit [ ' SOURCE cell+ !
variable >IN

\ anonymous definitions
: delimiter? ( delim char -- flag )
    over bl = if nip 127 33 within else = then
;
: skip-delimiters ( char "<chars>" -- )
    >r source swap >r				( R: char c-addr )
    >in @ 1-
    begin 1+ 2dup > while			( end idx )
	dup chars 2r@ rot + c@			( end idx char c )
	delimiter? invert
    until then >in ! drop 2r@ 2drop
;

\ target definitions
: PARSE ( char "ccc<char>" -- c-addr u )
    >r source swap >in @ chars + >r		( R: delim c-addr )
    >in @ - -1
    begin 1+ 2dup > while			( end idx )
	dup chars 2r@ rot + c@			( end idx delim c )
	delimiter?
    until
	nip dup 1+
    then >in +! r> swap r> drop
;

\ anonymous definitions
: parse-name ( -- c-addr u ) bl skip-delimiters bl parse ;

\ label word-addr - >= 33 characters, <= size of counted string

\ target definitions
: WORD ( char "<chars>ccc<char>" -- c-addr )
    parse-name word-addr 2dup c! char+
    swap chars 2dup + bl c! move
;

: CHAR ( "<spaces>name" --- char ) parse-name drop c@ ;
: ( [char] ) parse 2drop ; immediate

\ ------  ------  ------  ------  ------  ------  ------  ------
\ .(                    6.2.0200 CORE EXT               49
\ PARSE                 6.2.2008 CORE EXT               55
\ REFILL                6.2.2125 CORE EXT               55
\ RESTORE-INPUT         6.2.2148 CORE EXT               56
\ SAVE-INPUT            6.2.2182 CORE EXT               56
\ SOURCE-ID             6.2.2218 CORE EXT               56
\ \                     6.2.2535 CORE EXT               58
\ ------  ------  ------  ------  ------  ------  ------  ------

\ input source specification
\   common data -
\     source-id
\     start address
\     current size
\     >in
\   terminal extends common data -
\     max size
\   seekable file extends terminal -
\     offset of buffer w/in file
\   block data extends common data -
\     ???
\ REFILL depends on input source
: .( [char] ) parse type ; immediate
: \ source >in ! drop ; immediate
