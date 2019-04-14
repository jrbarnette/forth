\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  >IN                   6.1.0560 CORE                   31
\  SOURCE                6.1.2216 CORE                   45
\
\  REFILL                6.2.2125 CORE EXT               55
\  SOURCE-ID             6.2.2218 CORE EXT               56
\ ------  ------  ------  ------  ------  ------  ------  ------

<C> &DICT.to_in;   constant >IN

: SOURCE   ( -- c-addr u )  <C> &DICT.source;  2@ ;

prim: REFILL-TERMINAL   x_refill_terminal

: SOURCE-ID   <C> &DICT.source_id; @ ;

: SOURCE-DATA@
    source <C> &DICT.source_max_len; @ >in @ source-id ;
: SOURCE-DATA!
    <C> &DICT.source_id; ! >in ! <C> &DICT.source_max_len; !
    <C> &DICT.source; 2! ;

: NEST-SOURCE    r> source-data@ >r >r >r 2>r >r ;
: UNNEST-SOURCE  r> 2r> r> r> r> source-data! >r ;

: SOURCE<TERMINAL ( source-addr #source -- ) 0 swap 0 0 source-data! ;
: SOURCE<EVALUATE ( c-addr u -- ) 0 0 -1 source-data! ;

here 80 chars allot constant PROMPT 0 prompt c!
: PROMPT! ( c-addr u -- ) prompt swap chars 2dup + >r move 0 r> c! ;
: REFILL
    source-id 0< if false exit then
    <C> &DICT.source.c_addr; @ <C> &DICT.source_max_len; @ prompt
    refill-terminal <C> &DICT.source.len; ! dup if 0 >in ! then ;
