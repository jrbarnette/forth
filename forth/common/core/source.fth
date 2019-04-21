\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  >IN                   6.1.0560 CORE
\  SOURCE                6.1.2216 CORE
\
\  REFILL                6.2.2125 CORE EXT
\  SOURCE-ID             6.2.2218 CORE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------

\ >IN CELL+ @       ->  SOURCE-ID
\ >IN 2 CELLS + 2@  ->  SOURCE
\ >IN 2 CELLS + @   ->  input buffer length
\ >IN 3 CELLS + @   ->  input buffer address
\ >IN 4 CELLS + @   ->  input buffer allotted size

variable >IN 4 cells allot

: SOURCE-ID  [ >in cell+ ] literal @ ;

: SOURCE  ( -- c-addr u )  [ >in 2 cells + ] literal 2@ ;

: SOURCE-DATA@  ( -- buffer len max-len >in source-id )
    source [ >in 4 cells + ] literal @ >in @ source-id ;
: SOURCE-DATA!  ( buffer len max-len >in source-id -- )
    [ >in cell+ ] literal ! >in ! [ >in 4 cells + ] literal !
    [ >in 2 cells + ] literal 2! ;

: NEST-SOURCE    r> source-data@ >r >r >r 2>r >r ;
: UNNEST-SOURCE  r> 2r> r> r> r> source-data! >r ;

: SOURCE<EVALUATE  ( c-addr u -- ) 0 0 -1 source-data! ;

here 256 dup chars allot        ( TIB #TIB )
: SOURCE<TERMINAL  ( -- )
    [ rot ] literal 0 [ swap ] literal 0 0 source-data! ;


here 80 chars allot constant PROMPT 0 prompt c!
: PROMPT! ( c-addr u -- ) prompt swap chars 2dup + >r move 0 r> c! ;
: REFILL
    source-id 0< if false exit then
    [ >in 3 cells + ] literal @ [ >in 4 cells + ] literal @ prompt
    refill-terminal [ >in 2 cells + ] literal ! dup if 0 >in ! then ;
