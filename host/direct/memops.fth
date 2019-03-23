\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\  memops.fth - Standard Forth words for memory fetch/store, and
\    block memory operations.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  +!                    6.1.0130 CORE                   27
\  2!                    6.1.0310 CORE                   29
\  2@                    6.1.0350 CORE                   29
\  ALIGNED               6.1.0706 CORE                   33
\  CELL+                 6.1.0880 CORE                   35
\  CELLS                 6.1.0890 CORE                   35
\  CHAR+                 6.1.0897 CORE                   35
\  CHARS                 6.1.0898 CORE                   35
\  COUNT                 6.1.0980 CORE                   36
\  FILL                  6.1.1540 CORE                   39
\  MOVE                  6.1.1900 CORE                   42
\  ERASE                 6.2.1350 CORE EXT               53
\ ------  ------  ------  ------  ------  ------  ------  ------

\ CORE
: CHARS ;
: CHAR+ [ 1 chars ] literal + ;

: CELLS <C> CELL_SIZE; * ;
: CELL+ [ 1 cells ] literal + ;

\ : ALIGNED ( addr -- a-addr )
\     [ 1 cells 1- ] literal + [ -1 cells ] literal and ;
\ : +! ( x a-addr -- ) dup >r @ + r> ! ;
\ : 2! ( x1 x2 a-addr -- ) swap over ! cell+ ! ;
\ : 2@ ( a-addr -- x1 x2 ) dup cell+ @ swap @ ;
\ : COUNT ( c-addr -- c-addr u ) dup char+ swap c@ ;
