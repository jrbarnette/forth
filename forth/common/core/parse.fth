\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  >IN                   6.1.0560 CORE
\  BL                    6.1.0770 CORE
\  SOURCE                6.1.2216 CORE
\  WORD                  6.1.2450 CORE
\
\  PARSE                 6.2.2008 CORE EXT
\  PARSE-NAME            6.2.2008 CORE EXT (ANS 2012)
\  REFILL                6.2.2125 CORE EXT
\  SOURCE-ID             6.2.2218 CORE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------

32 constant BL

\ >IN CELL+ @       ->  SOURCE-ID
\ >IN 2 CELLS + 2@  ->  SOURCE
\ >IN 2 CELLS + @   ->  input buffer length
\ >IN 3 CELLS + @   ->  input buffer address
\ >IN 4 CELLS + @   ->  input buffer allotted size

variable >IN 4 cells allot

: SOURCE-ID  [ >in cell+ ] literal @ ;

: SOURCE  ( -- c-addr u )  [ >in 2 cells + ] literal 2@ ;

: SOURCE-DATA@
    source [ >in 4 cells + ] literal @ >in @ source-id ;
: SOURCE-DATA!
    [ >in cell+ ] literal ! >in ! [ >in 4 cells + ] literal !
    [ >in 2 cells + ] literal 2! ;

: NEST-SOURCE    r> source-data@ >r >r >r 2>r >r ;
: UNNEST-SOURCE  r> 2r> r> r> r> source-data! >r ;

: SOURCE<TERMINAL  ( source-addr #source -- ) 0 swap 0 0 source-data! ;
: SOURCE<EVALUATE  ( c-addr u -- ) 0 0 -1 source-data! ;

variable >IN-START
: PARSE ( char "ccc<char>" -- c-addr u )
    >in @ >in-start !
    >r source swap >in @ chars + >r		( R: delim c-addr )
    >in @ - -1
    begin 1+ 2dup > while			( end cnt )
	dup 2r@ rot chars + c@			( end cnt delim c )
	over bl = if nip graphic? 0= else = then
    until
	nip dup 1+
    then >in +! r> swap r> drop
;

\ Oh my, I'm surely going to pay for this...
: WORD  here dup rot parse counted, bl c, here - allot ;

\ CORE EXT (2012 standard)
: PARSE-NAME
    source swap >r >in @ 1-                     ( end idx ) ( R: c-addr )
    begin 1+ 2dup > while			( end idx )
	r@ over chars + c@ graphic?
    until then >in ! drop r> drop bl parse
;

here 80 chars allot constant PROMPT 0 prompt c!
: PROMPT! ( c-addr u -- ) prompt swap chars 2dup + >r move 0 r> c! ;
: REFILL
    source-id 0< if false exit then
    [ >in 3 cells + ] literal @ [ >in 4 cells + ] literal @ prompt
    refill-terminal [ >in 2 cells + ] literal ! dup if 0 >in ! then ;
