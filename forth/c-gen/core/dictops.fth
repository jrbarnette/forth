\ Copyright 2020, by J. Richard Barnette. All Rights Reserved.

\  dictops.fth - Definitions for standard Forth words for dictionary
\      management.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  ALIGN                 6.1.0705 CORE
\  C,                    6.1.0860 CORE
\
\  PAD                   6.2.2000 CORE EXT
\  UNUSED                6.2.2395 CORE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------

: ALIGN ( -- ) here aligned here-addr ! ;
: C, ( char -- ) here [ 1 chars ] literal allot c! ;

: PAD ( -- c-addr )  here [ 96 cells ] literal + ;

: UNUSED ( -- u ) <C> DICTSPACE_END; here - ;
