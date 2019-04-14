\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  >IN                   6.1.0560 CORE                   31
\  SOURCE                6.1.2216 CORE                   45
\
\  REFILL                6.2.2125 CORE EXT               55
\  SOURCE-ID             6.2.2218 CORE EXT               56
\ ------  ------  ------  ------  ------  ------  ------  ------

prim: REFILL-TERMINAL   x_refill_terminal

\ >IN CELL+ @       ->  SOURCE-ID
\ >IN 2 CELLS + 2@  ->  SOURCE
\ >IN 2 CELLS + @   ->  input buffer length
\ >IN 3 CELLS + @   ->  input buffer address
\ >IN 4 CELLS + @   ->  input buffer allotted size

variable >IN 4 cells allot

: SOURCE   ( -- c-addr u )  [ >in 2 cells + ] literal 2@ ;

: SOURCE-ID   [ >in cell+ ] literal @ ;

: SOURCE-DATA@
    source [ >in 4 cells + ] literal @ >in @ source-id ;
: SOURCE-DATA!
    [ >in cell+ ] literal ! >in ! [ >in 4 cells + ] literal !
    [ >in 2 cells + ] literal 2! ;

: NEST-SOURCE    r> source-data@ >r >r >r 2>r >r ;
: UNNEST-SOURCE  r> 2r> r> r> r> source-data! >r ;

: SOURCE<TERMINAL ( source-addr #source -- ) 0 swap 0 0 source-data! ;
: SOURCE<EVALUATE ( c-addr u -- ) 0 0 -1 source-data! ;

here 80 chars allot constant PROMPT 0 prompt c!
: PROMPT! ( c-addr u -- ) prompt swap chars 2dup + >r move 0 r> c! ;
: REFILL
    source-id 0< if false exit then
    [ >in 3 cells + ] literal @ [ >in 4 cells + ] literal @ prompt
    refill-terminal [ >in 2 cells + ] literal ! dup if 0 >in ! then ;
