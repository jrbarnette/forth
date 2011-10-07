\ ------  ------  ------  ------  ------  ------  ------
\ index:
\ +                     6.1.0120 CORE                   26
\ -                     6.1.0160 CORE                   27
\ 0<                    6.1.0250 CORE                   28
\ 0=                    6.1.0270 CORE                   28
\ 1+                    6.1.0290 CORE                   28
\ 1-                    6.1.0300 CORE                   29
\ 2*                    6.1.0320 CORE                   29
\ 2/                    6.1.0330 CORE                   29
\ <                     6.1.0480 CORE                   30
\ =                     6.1.0530 CORE                   31
\ >                     6.1.0540 CORE                   31
\ ABS                   6.1.0690 CORE                   32
\ AND                   6.1.0720 CORE                   33
\ INVERT                6.1.1720 CORE                   41
\ LSHIFT                6.1.1805 CORE                   42
\ MAX                   6.1.1870 CORE                   42
\ MIN                   6.1.1880 CORE                   42
\ NEGATE                6.1.1910 CORE                   43
\ OR                    6.1.1980 CORE                   43
\ RSHIFT                6.1.2162 CORE                   45
\ U<                    6.1.2340 CORE                   47
\ XOR                   6.1.2490 CORE                   49
\ 0<>                   6.2.0260 CORE EXT               51
\ 0>                    6.2.0280 CORE EXT               51
\ <>                    6.2.0500 CORE EXT               52
\ U>                    6.2.2350 CORE EXT               59
\ WITHIN                6.2.2440 CORE EXT               60

\ ------  ------  ------  ------  ------  ------  ------
\ primitive unary and binary operations - required
\ + ( x1 x2 -- x3 )
\ - ( x1 x2 -- x3 )
\ 2* ( x1 -- x1 )
\ 2/ ( x1 -- x1 )
\ < ( n1 n2 -- flag )
\ = ( x1 x2 -- flag )
\ > ( n1 n2 -- flag )
\ AND ( x1 x2 -- x3 )
\ INVERT ( x1 -- x2 )
\ LSHIFT ( x1 u -- x2 )
\ NEGATE ( n1 -- n2 )
\ OR ( x1 x2 -- x3 )
\ RSHIFT ( x1 u -- x2 )
\ U< ( u1 u2 -- flag )
\ XOR ( x1 x2 -- x3 )

\ utility unary and binary operations
: 0< ( n -- flag ) 0 < ;
: 0= ( n -- flag ) 0 = ;
: 1+ ( x1 -- x2 ) 1 + ;
: 1- ( x1 -- x2 ) 1 - ;
: ABS ( n -- u ) dup 0< if negate then ;
: MAX ( n1 n2 -- n3 ) 2dup < if swap then drop ;
: MAX ( n1 n2 -- n3 ) 2dup > if swap then drop ;

\ ------  ------  ------  ------  ------  ------  ------
\ primitive unary and binary operations - extensions
\ <> ( x1 x2 -- flag )
\ U> ( u1 u2 -- flag )

\ utility unary and binary operations
: 0<> ( n -- flag ) 0 <> ;
: 0> ( n -- flag ) 0 > ;
: WITHIN ( x1 x2 x3 -- flag ) over - >r - r> u< ;
