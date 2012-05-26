\ Copyright 2011, by J. Richard Barnette
\ ------  ------  ------  ------  ------  ------  ------
\ anonymous defintions
\ XXX this must be shared with host definitions
variable here-addr
\ target definitions

\ dictionary data allocation primitives - required
\ HERE                  6.1.1650 CORE                   40
: HERE ( -- addr ) here-addr @ ;

\ ALLOT                 6.1.0710 CORE                   33
: ALLOT ( n -- ) here-addr @ + here-addr ! ;

\ ALIGN                 6.1.0705 CORE                   33
: ALIGN ( -- ) here aligned here-addr ! ;

\ ,                     6.1.0150 CORE                   27
: , ( x -- ) here [ 1 cells ] literal allot ! ;

\ C,                    6.1.0860 CORE                   34
: C, ( char -- ) here [ 1 chars ] literal allot c! ;

\ ------  ------  ------  ------  ------  ------  ------
\ dictionary allocation primitives - extensions
\ PAD                   6.2.2000 CORE EXT               56
\ PAD ( -- c-addr )
\ UNUSED                6.2.2395 CORE EXT               59
\ UNUSED ( -- u )
