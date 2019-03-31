\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\ 0<                    6.1.0250 CORE                   28
\ 0=                    6.1.0270 CORE                   28
\ 1+                    6.1.0290 CORE                   28
\ 1-                    6.1.0300 CORE                   29
\ ABS                   6.1.0690 CORE                   32
\ MAX                   6.1.1870 CORE                   42
\ MIN                   6.1.1880 CORE                   42
\ S>D                   6.1.2170 CORE                   44
\
\ 0<>                   6.2.0260 CORE EXT               49
\ 0>                    6.2.0280 CORE EXT               50
\ <>                    6.2.0500 CORE EXT               50
\ FALSE                 6.2.1485 CORE EXT               54
\ TRUE                  6.2.2298 CORE EXT               57
\ U>                    6.2.2350 CORE EXT               57
\ WITHIN                6.2.2440 CORE EXT               58
\ ------  ------  ------  ------  ------  ------  ------  ------

: 0< ( n -- flag ) 0 < ;
: 0= ( n -- flag ) 0 = ;
: 1+ ( x1 -- x2 ) 1 + ;
: 1- ( x1 -- x2 ) 1 - ;
: ABS ( n -- u ) dup 0< if negate then ;
: MAX ( n1 n2 -- n3 ) 2dup < if swap then drop ;
: MIN ( n1 n2 -- n3 ) 2dup > if swap then drop ;
: S>D ( n -- d ) dup 0< ;


\ ------  ------  ------  ------  ------  ------  ------  ------
\ ------  ------  ------  ------  ------  ------  ------  ------

: <> ( n1 n2 -- flag ) = invert ;
: 0> ( n -- flag ) 0 > ;
: 0<> ( n -- flag ) 0 <> ;

1 0 = constant FALSE
0 0 = constant TRUE

: U> ( n1 n2 -- flag ) swap u< ;
: WITHIN ( x1 x2 x3 -- flag ) over - >r - r> u< ;
