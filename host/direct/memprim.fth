\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\  memprim.fth - Standard Forth words for memory fetch/store, and
\    block memory operations.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  !                     6.1.0010 CORE                   25
\  +!                    6.1.0130 CORE                   27
\  2!                    6.1.0310 CORE                   29
\  2@                    6.1.0350 CORE                   29
\  @                     6.1.0650 CORE                   32
\  ALIGNED               6.1.0706 CORE                   33
\  C!                    6.1.0850 CORE                   34
\  C@                    6.1.0870 CORE                   34
\  CELL+                 6.1.0880 CORE                   35
\  CELLS                 6.1.0890 CORE                   35
\  CHAR+                 6.1.0897 CORE                   35
\  CHARS                 6.1.0898 CORE                   35
\  COUNT                 6.1.0980 CORE                   36
\  FILL                  6.1.1540 CORE                   39
\  MOVE                  6.1.1900 CORE                   42
\ 
\  ERASE                 6.2.1350 CORE EXT               53
\ ------  ------  ------  ------  ------  ------  ------  ------

\ CORE
prim: !         x_store
\     +!
\     2!
\     2@
prim: @         x_fetch
\     ALIGNED
prim: C!        x_c_store
prim: C@        x_c_fetch
\     CELL+
\     CELLS
\     CHAR+
\     CHARS
\     COUNT
prim: FILL	x_fill
prim: MOVE      x_move

\ CORE EXT
\     ERASE
