\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\ (                     6.1.0080 CORE                   26
\ >IN                   6.1.0560 CORE                   31
\ SOURCE                6.1.2216 CORE                   45
\ ------  ------  ------  ------  ------  ------  ------  ------

variable >IN
    3 cells allot
: SOURCE ( -- c-addr u ) [ >in 2 cells + ] literal 2@ ;
: SOURCE! ( c-addr u -- ) [ >in 2 cells + ] literal 2! ;
: SOURCE-ID [ >in cell+ ] literal @ ;
: SOURCE-ID! [ >in cell+ ] literal ! ;

\ ------  ------  ------  ------  ------  ------  ------  ------
\ .(                    6.2.0200 CORE EXT               49
\ PARSE                 6.2.2008 CORE EXT               55
\ REFILL                6.2.2125 CORE EXT               55
\ RESTORE-INPUT         6.2.2148 CORE EXT               56
\ SAVE-INPUT            6.2.2182 CORE EXT               56
\ SOURCE-ID             6.2.2218 CORE EXT               56
\ \                     6.2.2535 CORE EXT               58
\ ------  ------  ------  ------  ------  ------  ------  ------

: PARSE ( char "ccc<char>" -- c-addr u )
    >r source swap >in @ chars + >r		( R: delim c-addr )
    >in @ - -1
    begin 1+ 2dup > while			( end cnt )
	dup 2r@ rot chars + c@			( end cnt delim c )
	over bl = if nip 127 33 within else = then
    until
	nip dup 1+
    then >in +! r> swap r> drop
;

\ non-standard definition
: PARSE-WORD  ( char "<chars>ccc<char>" -- c-addr u )
    >r source swap >r				( R: delim c-addr )
    >in @ 1-
    begin 1+ 2dup > while			( end idx )
	dup 2r@ rot chars + c@			( end idx char c )
	over bl = if nip 33 127 within else <> then
    until then >in ! drop 2r> drop parse
;

: WORD ( char "<chars>ccc<char>" -- c-addr )
    parse-word here aligned cell+		( src u dst )
    dup >r					( R: c-addr )
    2dup c! char+ swap chars			( src dst' u' )
    2dup + >r move bl r> c! r>			( c-addr )
;

here 256 dup chars allot
pad aligned 2!
: REFILL ( -- flag )
    source-id if false exit then
    [ pad aligned 2@ swap ] literal dup literal	( c-addr c-addr +n1 )
    do-accept if drop false exit then		( c-addr +n2 )
    source! 0 >in true
;
