\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  >IN                   6.1.0560 CORE
\  SOURCE                6.1.2216 CORE
\
\  REFILL                6.2.2125 CORE EXT
\  SOURCE-ID             6.2.2218 CORE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------

\ >IN CELL+         ->  SOURCE-ID
\ >IN 2 CELLS +     ->  SOURCE len
\ >IN 3 CELLS +     ->  SOURCE buffer address
\ >IN 4 CELLS +     ->  input buffer allotted size
\ >IN 5 CELLS +     ->  end of input source specification

variable >IN 4 cells allot

: SOURCE-ID  [ >in cell+ ] literal @ ;
: SOURCE  ( -- c-addr u )  [ >in 2 cells + ] literal 2@ ;

: NEST-SOURCE
    r> >in [ >in 5 cells + ] literal
    begin 2dup < while 1 cells - dup @ >r repeat 2drop >r ;
: UNNEST-SOURCE
    r> [ >in 5 cells + ] literal >in
    begin 2dup > while r> over ! cell+ repeat 2drop >r ;

: SOURCE-ID! >in 0 over ! cell+ ! ;
: SOURCE<EVALUATE  ( c-addr u -- )
    [ >in 2 cells + ] literal 2! -1 source-id! ;

here 256 dup chars allot        ( TIB #TIB )
: SOURCE<TERMINAL  ( -- )
    [ swap ] literal [ swap ] literal ( #TIB TIB )
    [ >in 3 cells + ] literal 2! 0 source-id! ;

here 80 chars allot constant PROMPT 0 prompt c!
: PROMPT! ( c-addr u -- ) prompt swap chars 2dup + >r move 0 r> c! ;
: REFILL
    source-id 0< if false exit then
    [ >in 3 cells + ] literal @ [ >in 4 cells + ] literal @ prompt
    refill-terminal [ >in 2 cells + ] literal ! dup if 0 >in ! then ;
