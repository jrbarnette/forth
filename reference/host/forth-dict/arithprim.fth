\ Copyright 2015, by J. Richard Barnette, All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  +                     6.1.0120 CORE                   26
\  -                     6.1.0160 CORE                   27
\  0<                    6.1.0250 CORE                   28
\  0=                    6.1.0270 CORE                   28
\  1+                    6.1.0290 CORE                   28
\  1-                    6.1.0300 CORE                   29
\  2*                    6.1.0320 CORE                   29
\  2/                    6.1.0330 CORE                   29
\  <                     6.1.0480 CORE                   30
\  =                     6.1.0530 CORE                   31
\  >                     6.1.0540 CORE                   31
\  AND                   6.1.0720 CORE                   33
\  INVERT                6.1.1720 CORE                   40
\  LSHIFT                6.1.1805 CORE                   41
\  NEGATE                6.1.1910 CORE                   42
\  OR                    6.1.1980 CORE                   42
\  RSHIFT                6.1.2162 CORE                   44
\  U<                    6.1.2340 CORE                   46
\  XOR                   6.1.2490 CORE                   48
\ 
\  0<>                   6.2.0260 CORE EXT               49
\  0>                    6.2.0280 CORE EXT               50
\  <>                    6.2.0500 CORE EXT               50
\  U>                    6.2.2350 CORE EXT               57
\ ------  ------  ------  ------  ------  ------  ------  ------

\ : 0< 0 < ;
\ : 0= 0 = ;
\ : 1+ 1 + ;
\ : 1- 1 - ;
\ : 2* 1 LSHIFT ;

prim: +
prim: -
prim: 0<
prim: 0=
prim: 1+
prim: 1-
prim: 2*
prim: 2/
prim: <
prim: =
prim: >
prim: AND
prim: INVERT
prim: LSHIFT
prim: NEGATE
prim: OR
prim: RSHIFT
prim: U<
prim: XOR

\ : <> = NEGATE ;
\ : 0<> 0 <> ;
\ : 0> 0 > ;
\ : U> SWAP U< ;

prim: 0<>
prim: 0>
prim: <>
prim: U>
