\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\ ,                     6.1.0150 CORE                   27
\ ALIGN                 6.1.0705 CORE                   33
\ ALLOT                 6.1.0710 CORE                   33
\ C,                    6.1.0860 CORE                   34
\ HERE                  6.1.1650 CORE                   39
\ ------  ------  ------  ------  ------  ------  ------  ------

\ N.B. host compiler must define `here-addr` that compiles the
\ address of the HERE pointer (preferably as a literal).

: HERE ( -- addr ) here-addr @ ;
: ALLOT ( n -- ) here-addr +! ;
: ALIGN ( -- ) here aligned here-addr ! ;
: , ( x -- ) here [ 1 cells ] literal allot ! ;
: C, ( char -- ) here [ 1 chars ] literal allot c! ;


\ ------  ------  ------  ------  ------  ------  ------  ------
\ PAD                   6.2.2000 CORE EXT               55
\ UNUSED                6.2.2395 CORE EXT               57
\ ------  ------  ------  ------  ------  ------  ------  ------

\ not yet defined - could be forth
\ PAD ( -- c-addr )
\ UNUSED ( -- u )
