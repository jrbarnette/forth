\ Copyright 2016, by J. Richard Barnette. All Rights Reserved.

\  arithprim.fth - Standard Forth words for single cell arithmetic
\    and logical operations.

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
\  ABS                   6.1.0690 CORE                   32
\  AND                   6.1.0720 CORE                   33
\  INVERT                6.1.1720 CORE                   40
\  LSHIFT                6.1.1805 CORE                   41
\  MAX                   6.1.1870 CORE                   42
\  MIN                   6.1.1880 CORE                   42
\  NEGATE                6.1.1910 CORE                   42
\  OR                    6.1.1980 CORE                   42
\  RSHIFT                6.1.2162 CORE                   44
\  S>D                   6.1.2170 CORE                   44
\  U<                    6.1.2340 CORE                   46
\  XOR                   6.1.2490 CORE                   48
\
\  0<>                   6.2.0260 CORE EXT               49
\  0>                    6.2.0280 CORE EXT               50
\  <>                    6.2.0500 CORE EXT               50
\  U>                    6.2.2350 CORE EXT               57
\  WITHIN                6.2.2440 CORE EXT               58
\ ------  ------  ------  ------  ------  ------  ------  ------

\ CORE
prim: +         x_plus
prim: -         x_minus
\     0<
\     0=
\     1+
\     1-
prim: 2*        x_two_star
prim: 2/        x_two_slash
prim: <         x_less_than
prim: =         x_equals
prim: >         x_greater_than
\     ABS
prim: AND       x_and
prim: INVERT    x_invert
prim: LSHIFT    x_lshift
\     MAX
\     MIN
prim: NEGATE    x_negate
prim: OR        x_or
prim: RSHIFT    x_rshift
\     S>D
prim: U<        x_u_less
prim: XOR       x_xor

\ CORE EXT
\     0<>
\     0>
\     <>
\     U>
\     WITHIN