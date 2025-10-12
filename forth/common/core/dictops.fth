\ Copyright 2023, by J. Richard Barnette. All Rights Reserved.

\  dictops.fth - Definitions for standard Forth words for dictionary
\      management.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  ALIGN                 6.1.0705 CORE
\  C,                    6.1.0860 CORE
\ ------  ------  ------  ------  ------  ------  ------  ------

: ALIGN ( -- ) here aligned here-addr ! ;
: C, ( char -- ) here [ 1 chars ] literal allot c! ;
