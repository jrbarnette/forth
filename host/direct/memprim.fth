\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\  memprim - Forth primitive definitions for memory fetch/store, and
\    block memory operations.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  !                     6.1.0010 CORE                   25
\  @                     6.1.0650 CORE                   32
\  C!                    6.1.0850 CORE                   34
\  C@                    6.1.0870 CORE                   34
\  FILL                  6.1.1540 CORE                   39
\  MOVE                  6.1.1900 CORE                   42
\ ------  ------  ------  ------  ------  ------  ------  ------

prim: !         x_store     ( x a-addr -- )
prim: @         x_fetch     ( a-addr -- x )
prim: C!        x_c_store   ( char c-addr -- )
prim: C@        x_c_fetch   ( c-addr -- char )
prim: FILL	x_fill      ( c-addr u char -- )
prim: MOVE      x_move      ( addr1 addr2 u -- )
