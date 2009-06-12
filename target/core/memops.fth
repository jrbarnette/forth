\ ------  ------  ------  ------  ------  ------  ------
\ cell size and storage primitives
\ !                     6.1.0010 CORE                   25
\ @                     6.1.0650 CORE                   32
\ ALIGNED               6.1.0706 CORE                   33
\ C!                    6.1.0850 CORE                   34
\ C@                    6.1.0870 CORE                   34
\ CELL+                 6.1.0880 CORE                   35
\ CELLS                 6.1.0890 CORE                   35
\ CHAR+                 6.1.0897 CORE                   35
\ CHARS                 6.1.0898 CORE                   35
\ FILL                  6.1.1540 CORE                   39
\ MOVE                  6.1.1900 CORE                   43

\ +!                    6.1.0130 CORE                   27
: +! ( x a-addr -- ) dup >r @ + r> ! ;
\ : +! ( x a-addr -- ) dup @ rot + swap ! ;

\ 2!                    6.1.0310 CORE                   29
: 2! ( x1 x2 a-addr -- ) swap over ! cell+ ! ;

\ 2@                    6.1.0350 CORE                   29
: 2@ ( a-addr -- x1 x2 ) dup cell+ @ swap @ ;

\ ERASE                 6.2.1350 CORE EXT               55
\ FIXME - doesn't work if characters need alignment
: ERASE ( addr u - ) chars 0 fill ;
