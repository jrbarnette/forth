\ Copyright 2015, by J. Richard Barnette, All Rights Reserved.

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
\  PICK                  6.2.2030 CORE EXT               55
\ ------  ------  ------  ------  ------  ------  ------  ------

\ : 2DROP DROP DROP ;
\ : 2DUP OVER OVER ;

prim: 2DROP     x_two_drop
prim: 2DUP      x_two_dup
prim: 2OVER     x_two_over
prim: 2SWAP     x_two_swap
prim: >R        x_to_r			no-interpret
prim: ?DUP      x_question_dup
prim: DEPTH     x_depth
prim: DROP      x_drop
prim: DUP       x_dup
prim: OVER      x_over
prim: R>        x_r_from		no-interpret
prim: R@        x_r_fetch		no-interpret
prim: ROT       x_rot
prim: SWAP      x_swap

prim: 2>R       x_two_to_r		no-interpret
prim: 2R>       x_two_r_from		no-interpret
prim: 2R@       x_two_r_fetch		no-interpret
prim: PICK      x_pick
