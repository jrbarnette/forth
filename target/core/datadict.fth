\ ------  ------  ------  ------  ------  ------  ------
\ index:
\ ,                     6.1.0150 CORE                   27
\ ALIGN                 6.1.0705 CORE                   33
\ ALLOT                 6.1.0710 CORE                   33
\ C,                    6.1.0860 CORE                   34
\ HERE                  6.1.1650 CORE                   40
\ PAD                   6.2.2000 CORE EXT               56
\ UNUSED                6.2.2395 CORE EXT               59

\ ------  ------  ------  ------  ------  ------  ------
\ dictionary data allocation primitives - required
\ HERE ( -- addr )
\ ALLOT ( n -- )
: ALIGN ( -- ) HERE ALIGNED HERE - ALLOT ;

: , ( x -- ) HERE [ 1 CELLS ] LITERAL ALLOT ! ;
: C, ( char -- ) HERE [ 1 CHARS ] LITERAL ALLOT C! ;

\ ------  ------  ------  ------  ------  ------  ------
\ dictionary allocation primitives - extensions
\ PAD ( -- c-addr )
\ UNUSED ( -- u )
