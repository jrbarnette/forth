\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\  memops - Standard Forth words for memory fetch/store, address
\    arithmetic, and block memory operations.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  CELL+                 6.1.0880 CORE                   35
\  CELLS                 6.1.0890 CORE                   35
\  CHAR+                 6.1.0897 CORE                   35
\  CHARS                 6.1.0898 CORE                   35
\ ------  ------  ------  ------  ------  ------  ------  ------

: CHARS ;
: CHAR+ 1 + ;

: CELLS <C> CELL_SIZE; * ;
: CELL+ <C> CELL_SIZE; + ;

: ALIGNED ( addr -- a-addr ) <C> CELL_SIZE-1; + <C> -CELL_SIZE; and ;
