\ ------  ------  ------  ------  ------  ------  ------
\ primitive unary and binary operations - required
\ +                     6.1.0120 CORE                   26
\ -                     6.1.0160 CORE                   27
\ 2*                    6.1.0320 CORE                   29
\ 2/                    6.1.0330 CORE                   29
\ <                     6.1.0480 CORE                   30
\ =                     6.1.0530 CORE                   31
\ >                     6.1.0540 CORE                   31
\ AND                   6.1.0720 CORE                   33
\ INVERT                6.1.1720 CORE                   41
\ LSHIFT                6.1.1805 CORE                   42
\ NEGATE                6.1.1910 CORE                   43
\ OR                    6.1.1980 CORE                   43
\ RSHIFT                6.1.2162 CORE                   45
\ U<                    6.1.2340 CORE                   47
\ XOR                   6.1.2490 CORE                   49

\ utility unary and binary operations
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
: MAX ( n1 n2 -- n3 ) 2dup > if swap then drop ;

\ ------  ------  ------  ------  ------  ------  ------
\ primitive unary and binary operations - extensions
\ <>                    6.2.0500 CORE EXT               52
\ U>                    6.2.2350 CORE EXT               59

\ utility unary and binary operations
\ 0<>                   6.2.0260 CORE EXT               51
: 0<> ( n -- flag ) 0 <> ;

\ 0>                    6.2.0280 CORE EXT               51
: 0> ( n -- flag ) 0 > ;

\ WITHIN                6.2.2440 CORE EXT               60
: WITHIN ( x1 x2 x3 -- flag ) over - >r - r> u< ;
