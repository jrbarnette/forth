\ Copyright 2016, by J. Richard Barnette. All Rights Reserved.

\  stackprim.fth - Standard Forth words for stack operations.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  2DROP                 6.1.0370 CORE                   29
\  2DUP                  6.1.0380 CORE                   29
\  2OVER                 6.1.0400 CORE                   29
\  2SWAP                 6.1.0430 CORE                   30
\  >R                    6.1.0580 CORE                   32
\  ?DUP                  6.1.0630 CORE                   32
\  DEPTH                 6.1.1200 CORE                   36
\  DROP                  6.1.1260 CORE                   37
\  DUP                   6.1.1290 CORE                   37
\  OVER                  6.1.1990 CORE                   42
\  R>                    6.1.2060 CORE                   43
\  R@                    6.1.2070 CORE                   43
\  ROT                   6.1.2160 CORE                   44
\  SWAP                  6.1.2260 CORE                   45
\
\  2>R                   6.2.0340 CORE EXT               50
\  2R>                   6.2.0410 CORE EXT               50
\  2R@                   6.2.0415 CORE EXT               50
\  NIP                   6.2.1930 CORE EXT               54
\  PICK                  6.2.2030 CORE EXT               55
\  ROLL                  6.2.2150 CORE EXT               56
\  TUCK                  6.2.2300 CORE EXT               57
\ ------  ------  ------  ------  ------  ------  ------  ------

\ CORE
\     2DROP
\     2DUP
\     2OVER
\     2SWAP
prim: >R        x_to_r              no-interpret
prim: ?DUP      x_question_dup
prim: DEPTH     x_depth
prim: DROP      x_drop
prim: DUP       x_dup
prim: OVER      x_over
prim: R>        x_r_from            no-interpret
prim: R@        x_r_fetch           no-interpret
prim: ROT       x_rot
prim: SWAP      x_swap

\ CORE EXT
prim: 2>R       x_two_to_r          no-interpret
prim: 2R>       x_two_r_from        no-interpret
prim: 2R@       x_two_r_fetch       no-interpret
\     NIP
prim: PICK      x_pick
\     ROLL
\     TUCK
