\ Copyright 2011, by J. Richard Barnette
\ ------  ------  ------  ------  ------  ------  ------
\ primitive unary and binary operations - required
\ +                     6.1.0120 CORE                   26
\ + ( x1 x2 -- x3 )
\ -                     6.1.0160 CORE                   27
\ - ( x1 x2 -- x3 )
\ 2*                    6.1.0320 CORE                   29
\ 2* ( x1 -- x1 )
\ 2/                    6.1.0330 CORE                   29
\ 2/ ( x1 -- x1 )
\ <                     6.1.0480 CORE                   30
\ < ( n1 n2 -- flag )
\ =                     6.1.0530 CORE                   31
\ = ( x1 x2 -- flag )
\ >                     6.1.0540 CORE                   31
\ > ( n1 n2 -- flag )
\ AND                   6.1.0720 CORE                   33
\ AND ( x1 x2 -- x3 )
\ INVERT                6.1.1720 CORE                   40
\ INVERT ( x1 -- x2 )
\ LSHIFT                6.1.1805 CORE                   41
\ LSHIFT ( x1 u -- x2 )
\ NEGATE                6.1.1910 CORE                   42
\ NEGATE ( n1 -- n2 )
\ OR                    6.1.1980 CORE                   42
\ OR ( x1 x2 -- x3 )
\ RSHIFT                6.1.2162 CORE                   44
\ RSHIFT ( x1 u -- x2 )
\ U<                    6.1.2340 CORE                   46
\ U< ( u1 u2 -- flag )
\ XOR                   6.1.2490 CORE                   48
\ XOR ( x1 x2 -- x3 )

\ non-primitive unary and binary operations
\ 0<                    6.1.0250 CORE                   28
: 0< ( n -- flag ) 0 < ;

\ 0=                    6.1.0270 CORE                   28
: 0= ( n -- flag ) 0 = ;

\ 1+                    6.1.0290 CORE                   28
: 1+ ( x1 -- x2 ) 1 + ;

\ 1-                    6.1.0300 CORE                   29
: 1- ( x1 -- x2 ) 1 - ;

\ ABS                   6.1.0690 CORE                   32
: ABS ( n -- u ) dup 0< if negate then ;

\ MAX                   6.1.1870 CORE                   42
: MAX ( n1 n2 -- n3 ) 2dup < if swap then drop ;

\ MIN                   6.1.1880 CORE                   42
: MIN ( n1 n2 -- n3 ) 2dup > if swap then drop ;

\ S>D                   6.1.2170 CORE                   44
: S>D ( n -- d ) dup 0< ;

\ ------  ------  ------  ------  ------  ------  ------
\ primitive unary and binary operations - extensions
\ <>                    6.2.0500 CORE EXT               50
\ : <> ( x1 x2 -- flag ) = invert ;

\ U> ( u1 u2 -- flag )
\ U>                    6.2.2350 CORE EXT               57

\ utility unary and binary operations
\ 0<>                   6.2.0260 CORE EXT               49
: 0<> ( n -- flag ) 0 <> ;

\ 0>                    6.2.0280 CORE EXT               50
: 0> ( n -- flag ) 0 > ;

\ WITHIN                6.2.2440 CORE EXT               58
: WITHIN ( x1 x2 x3 -- flag ) over - >r - r> u< ;
