\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\  parse.fth - Forth code and primitives relating to input stream
\      parsing.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  >IN                   6.1.0560 CORE                   31
\  SOURCE                6.1.2216 CORE                   45
\
\  PARSE                 6.2.2008 CORE EXT               55
\  REFILL                6.2.2125 CORE EXT               55
\  SOURCE-ID             6.2.2218 CORE EXT               56
\ ------  ------  ------  ------  ------  ------  ------  ------

<C> &DICT.to_in;   constant >IN

: SOURCE   ( -- c-addr u )  <C> &DICT.source;  2@ ;
\ : SOURCE!  ( c-addr u -- )  <C> &DICT.source;  2! ;

prim: PARSE         x_parse
prim: REFILL        x_refill

: SOURCE-ID <C> &DICT.source_id; @ ;
\ : SOURCE-ID! <C> &DICT.source_id; ! ;
