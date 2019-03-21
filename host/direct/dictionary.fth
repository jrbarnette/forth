\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\  dictionary.fth - Primitives for standard Forth words for
\      dictionary allocation.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  ,                     6.1.0150 CORE                   27
\  ALIGN                 6.1.0705 CORE                   33
\  ALLOT                 6.1.0710 CORE                   33
\  C,                    6.1.0860 CORE                   34
\  HERE                  6.1.1650 CORE                   39
\
\  PAD                   6.2.2000 CORE EXT               55
\  UNUSED                6.2.2395 CORE EXT               57
\ ------  ------  ------  ------  ------  ------  ------  ------

\ CORE
prim: ,         x_comma
prim: ALIGN     x_align
prim: ALLOT     x_allot
prim: C,        x_c_comma
prim: HERE      x_here

\ CORE EXT
\     PAD - omitted
prim: UNUSED    x_unused
