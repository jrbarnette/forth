\ Copyright 2020, by J. Richard Barnette. All Rights Reserved.

\  dictionary.fth - Primitives for standard Forth words for
\      dictionary allocation.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  ,                     6.1.0150 CORE                   27
\  ALIGN                 6.1.0705 CORE                   33
\  ALLOT                 6.1.0710 CORE                   33
\  C,                    6.1.0860 CORE                   34
\  HERE                  6.1.1650 CORE                   39
\ ------  ------  ------  ------  ------  ------  ------  ------

\ CORE
: HERE ( -- addr )  do-literal [ ->here ] @ ;
: ALLOT ( n -- )    do-literal [ ->here ] +! ;
: ALIGN ( -- )      here aligned do-literal [ ->here ] ! ;
: , ( x -- )        here [ 1 cells ] literal allot ! ;
: C, ( char -- )    here [ 1 chars ] literal allot c! ;
