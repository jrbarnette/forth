\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\ ,                     6.1.0150 CORE                   27
\ ALIGN                 6.1.0705 CORE                   33
\ ALLOT                 6.1.0710 CORE                   33
\ C,                    6.1.0860 CORE                   34
\ HERE                  6.1.1650 CORE                   39
\ ------  ------  ------  ------  ------  ------  ------  ------

\ N.B. we assume some primitives are available from elsewhere:
\   dp:          address pointing to the current value of HERE
\   dict-start:  address of the end of the dictionary
\   dict-end:    address of the end of the dictionary
\   dict-size:   size of the dictionary

: HERE ( -- addr ) dp @ ;

\ : allot-bounds dict-end 1+ here - dup dict-size 1+ - ;
\ : allot-check allot-bounds within if -8 throw then ;
\ : ALLOT dup allot-check dp +! ;

: allot-bounds dict-end 1+ dict-start ;
: allot-check allot-bounds within -8 and throw ;
: ALLOT here + dup allot-check dp ! ;

: ALIGN ( -- ) here aligned dp ! ;
: , ( x -- ) here [ 1 cells ] literal allot ! ;
: C, ( char -- ) here [ 1 chars ] literal allot c! ;


\ ------  ------  ------  ------  ------  ------  ------  ------
\ PAD                   6.2.2000 CORE EXT               55
\ UNUSED                6.2.2395 CORE EXT               57
\ ------  ------  ------  ------  ------  ------  ------  ------

\ not yet defined - could be forth
\ PAD ( -- c-addr )
\ : UNUSED allot-bounds drop 1- ;
: UNUSED [ dict-end ] literal here - ;
