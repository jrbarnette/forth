\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\ 2DROP                 6.1.0370 CORE                   29
\ 2DUP                  6.1.0380 CORE                   29
\ 2OVER                 6.1.0400 CORE                   29
\ 2SWAP                 6.1.0430 CORE                   30
\ >R                    6.1.0580 CORE                   32
\ ?DUP                  6.1.0630 CORE                   32
\ DEPTH                 6.1.1200 CORE                   36
\ DROP                  6.1.1260 CORE                   37
\ DUP                   6.1.1290 CORE                   37
\ OVER                  6.1.1990 CORE                   42
\ R>                    6.1.2060 CORE                   43
\ R@                    6.1.2070 CORE                   43
\ ROT                   6.1.2160 CORE                   44
\ SWAP                  6.1.2260 CORE                   45
\ ------  ------  ------  ------  ------  ------  ------  ------

\ primitives
\ >R ( x -- ) ( R: -- x )
\ DEPTH ( -- +n )
\ DROP ( x -- )
\ DUP ( x -- x x )
\ R> ( -- x ) ( R: x -- )
\ : R@ ( -- x ) ( R: x -- x ) r> dup >r ;
\ SWAP ( x1 x2 -- x2 x1 )

\ primitives - could be in forth
\ : ?DUP ( x -- 0 | x x ) dup if dup then ;
\ : OVER ( x1 x2 -- x1 x2 x2 ) >r dup r> swap ;
\ : ROT ( x1 x2 x3 -- x2 x3 x1 ) >r swap r> swap ;

: 2DROP ( x1 x2 -- ) drop drop ;
: 2DUP ( x1 x2 -- x1 x2 x1 x2 ) over over ;
: 2SWAP ( x1 x2 x3 x4 -- x3 x4 x1 x2 ) rot >r rot r> ;
: 2OVER ( x1 x2 x3 x4 -- x1 x2 x3 x4 x1 x2 ) >r >r 2dup r> r> 2swap ;


\ ------  ------  ------  ------  ------  ------  ------  ------
\ 2>R                   6.2.0340 CORE EXT               50
\ 2R>                   6.2.0410 CORE EXT               50
\ 2R@                   6.2.0415 CORE EXT               50
\ NIP                   6.2.1930 CORE EXT               54
\ PICK                  6.2.2030 CORE EXT               55
\ ROLL                  6.2.2150 CORE EXT               56
\ TUCK                  6.2.2300 CORE EXT               57
\ ------  ------  ------  ------  ------  ------  ------  ------

\ primitives
\ PICK ( xu ... x1 x0 u -- xu ... x1 x0 xu )
\ ROLL ( xu xu-1 ... x0 u -- xu-1 ... x0 xu )

\ primitives - can't be forth because they use the return stack
\ : 2>R ( x1 x2 -- ) ( R: -- x1 x2 ) swap >r >r ;
\ : 2R> ( -- x1 x2 ) ( R: x1 x2 -- ) r> r> swap ;
\ : 2R@ ( -- x1 x2 ) ( R: x1 x2 -- x1 x2 ) r> r> 2dup >r >r swap ;

: NIP ( x1 x2 -- x2 ) swap drop ;
: TUCK ( x1 x2 -- x2 x1 x2 ) swap over ;
