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

\ >IN CELL+         ->  SOURCE-ID
\ >IN 2 CELLS +     ->  SOURCE len
\ >IN 3 CELLS +     ->  SOURCE buffer address
\ >IN 4 CELLS +     ->  input buffer allotted size
\ >IN 5 CELLS +     ->  input buffer file position
\ >IN 7 CELLS +     ->  end of input source specification

variable >IN 6 cells allot
>in 2 cells + constant SOURCE-ADDR
>in 5 cells + constant SOURCE-POS
>in 7 cells + constant SOURCE-END

: SOURCE-ID  [ >in cell+ ] literal @ ;
: SOURCE  ( -- c-addr u )  source-addr 2@ ;

: SOURCE-BUFFER  ( -- c-addr u )
    [ >in 3 cells + ] literal dup @ swap cell+ @ ;

: REFILL-FILE ( c-addr u -- flag len )  source-id read-line drop swap ;
: REFILL
    source-id 0< if false exit then
    source-buffer source-id 0= if
        refill-terminal
    else
        source-id file-position drop source-pos 2! refill-file
    then source-addr ! dup if 0 >in ! then ;
: RESTORE-SOURCE
    source-id 0> 0= if exit then
    source-pos 2@ source-id reposition-file drop
    source-buffer refill-file 2drop ;

: NEST-SOURCE
    r> >in source-end
    begin 2dup < while 1 cells - dup @ >r repeat 2drop >r ;
: UNNEST-SOURCE
    r> source-end >in
    begin 2dup > while r> over ! cell+ repeat 2drop >r restore-source ;

: SOURCE-ID!  ( id -- ) 0 >in 2! ;
: SOURCE<EVALUATE  ( c-addr u -- ) source-addr 2! -1 source-id! ;

: SOURCE-BUFFER!  ( id #buff buff -- )
    [ >in 3 cells + ] literal 2! source-id! ;

here 256 dup chars allot        ( TIB #TIB )
: SOURCE<TERMINAL  ( -- )
    0 [ swap ] literal [ swap ] literal source-buffer! ;

here 256 dup chars allot        ( filebuf #filebuf )
: SOURCE<FILE  ( fileid -- )
    [ swap ] literal [ swap ] literal source-buffer! ;
