\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  >IN                   6.1.0560 CORE
\  SOURCE                6.1.2216 CORE
\
\  SOURCE-ID             6.2.2218 CORE EXT
\  SOURCE-ID          11.6.1.2218 FILE
\
\  REFILL                6.2.2125 CORE EXT
\  REFILL             11.6.2.2125 FILE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------

\ Data for the "input source specification".  See 2.1 "Definition of
\ terms".  The data is laid out contiguously following >IN:
\ >IN CELL+         ->  SOURCE-ID
\ >IN 2 CELLS +     ->  SOURCE len
\ >IN 3 CELLS +     ->  SOURCE buffer address
\ >IN 4 CELLS +     ->  input buffer allotted size
\ >IN 5 CELLS +     ->  input buffer file position (2 cells)
\ >IN 7 CELLS +     ->  end of input source specification

variable >IN 6 cells allot
>in 2 cells + constant SOURCE-ADDR
>in 5 cells + constant SOURCE-POS

: SOURCE-ID  ( -- -1 | 0 | fileid ) [ >in cell+ ] literal @ ;
: SOURCE  ( -- c-addr u )  source-addr 2@ ;

: SOURCE-BUFFER  ( -- c-addr u )  [ >in 3 cells + ] literal 2@ swap ;

: REFILL-FILE ( buff-ptr len fileid -- len flag )
    dup file-position drop source-pos 2! read-line drop ;
: REFILL ( -- flag )
    source-id 0< if false exit then
    source-buffer source-id                        ( buff len id )
    ?dup if refill-file else refill-terminal then  ( len flag )
    swap source-addr ! 0 >in ! ;
: RESET-SOURCE
    source-pos 2@ source-id reposition-file drop ;
: SAVE-SOURCE
    source-id 0> if reset-source then ;
: RESTORE-SOURCE ( -- )
    source-id 0> 0= if exit then
    reset-source source-buffer source-id refill-file 2drop ;

256 chars here over allot	( #TIB TIB )
256 chars here over allot	( #TIB TIB #filebuf filebuf )
, ,	( files )
, ,	( terminal )
here 4 cells - constant SOURCE-BUFFERS

: SOURCE-ID! ( c-addr u -1 | 0 | fileid )
    dup 0 >in 2! dup 0< if
	drop source-addr
    else
	0= negate 2* cells source-buffers + 2@
	[ >in 3 cells + ] literal
    then ( x0 x1 addr ) 2! ;

: WITH-INPUT-SOURCE ( c-addr u -1 xt | 0 xt | fileid xt -- i*x )
    save-source
    [ >in 7 cells + ] literal >in ( source-end index-ptr )
    begin 2dup > while dup @ >r cell+ repeat 2drop
    >r source-id! r> catch
    >in [ >in 7 cells + ] literal ( source-begin index-ptr )
    begin 2dup < while 1 cells - r> over ! repeat 2drop
    restore-source throw ;
