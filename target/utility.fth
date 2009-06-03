\ +!                    6.1.0130 CORE                   27
\ 0<                    6.1.0250 CORE                   28
\ 0=                    6.1.0270 CORE                   28
\ 1+                    6.1.0290 CORE                   28
\ 1-                    6.1.0300 CORE                   29
\ 2!                    6.1.0310 CORE                   29
\ 2*                    6.1.0320 CORE                   29
\ 2/                    6.1.0330 CORE                   29
\ 2@                    6.1.0350 CORE                   29
\ ABS                   6.1.0690 CORE                   32
\ ALIGNED               6.1.0706 CORE                   33
\ BASE                  6.1.0750 CORE                   34
\ BL                    6.1.0770 CORE                   34
\ CELL+                 6.1.0880 CORE                   35
\ CELLS                 6.1.0890 CORE                   35
\ CHAR+                 6.1.0897 CORE                   35
\ CHARS                 6.1.0898 CORE                   35
\ COUNT                 6.1.0980 CORE                   36
\ DECIMAL               6.1.1170 CORE                   36
\ ENVIRONMENT?          6.1.1345 CORE                   38
\ FILL                  6.1.1540 CORE                   39
\ MAX                   6.1.1870 CORE                   42
\ MIN                   6.1.1880 CORE                   42
\ MOVE                  6.1.1900 CORE                   43
\ HEX                   6.2.1660 CORE EXT               55
\ 0<>                   6.2.0260 CORE EXT               51
\ 0>                    6.2.0280 CORE EXT               51
\ <>                    6.2.0500 CORE EXT               52
\ ERASE                 6.2.1350 CORE EXT               55
\ FALSE                 6.2.1485 CORE EXT               55
\ TRUE                  6.2.2298 CORE EXT               59
\ WITHIN                6.2.2440 CORE EXT               60

: 2SWAP ( x4 x3 x2 x1 -- x2 x1 x4 x3 ) rot >r rot r> ;
: +! ( n a-addr -- ) dup >r @ + r> ! ;
: ABS ( n -- u ) dup 0< if negate then ;
variable BASE 10 base !
20 constant BL
: DECIMAL 10 base ! ;
1 0= constant FALSE
0 0= constant TRUE
: COUNT ( c-addr -- c-addr u ) dup char+ swap c@ ;
: MAX ( n1 n2 -- n1 | n2 ) 2dup < if swap then drop ;
: MIN ( n1 n2 -- n1 | n2 ) 2dup > if swap then drop ;
