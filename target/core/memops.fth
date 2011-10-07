\ ------  ------  ------  ------  ------  ------  ------
\ index:
\ !                     6.1.0010 CORE                   25
\ +!                    6.1.0130 CORE                   27
\ 2!                    6.1.0310 CORE                   29
\ 2@                    6.1.0350 CORE                   29
\ ALIGNED               6.1.0706 CORE                   33
\ CELL+                 6.1.0880 CORE                   35
\ CELLS                 6.1.0890 CORE                   35
\ CHAR+                 6.1.0897 CORE                   35
\ CHARS                 6.1.0898 CORE                   35
\ FILL                  6.1.1540 CORE                   39
\ C!                    6.1.0850 CORE                   34
\ C@                    6.1.0870 CORE                   34
\ @                     6.1.0650 CORE                   32
\ MOVE                  6.1.1900 CORE                   43
\ ERASE                 6.2.1350 CORE EXT               55
\ ------  ------  ------  ------  ------  ------  ------

\ cell size and storage primitives - required
\ ! ( x a-addr -- )
\ @ ( a-addr -- x )
\ CELLS ( n1 -- n2 )
\ ALIGNED ( addr -- a-addr )

\ C! ( char c-addr -- )
\ C@ ( c-addr -- char ) 
\ CHARS ( n1 -- n2 )

: +! ( x a-addr -- ) DUP >R @ + R> ! ;
\ : +! ( x a-addr -- ) DUP @ ROT + SWAP ! ;

: CELL+ ( a-addr1 -- a-addr2 ) [ 1 CELLS ] LITERAL + ;
: CHAR+ ( c-addr1 -- c-addr2 ) [ 1 CHARS ] LITERAL + ;

: 2! ( x1 x2 a-addr -- ) SWAP OVER ! CELL+ ! ;
: 2@ ( a-addr -- x1 x2 ) DUP CELL+ @ SWAP @ ;

\ FILL ( c-addr u char -- )
\ MOVE ( addr1 addr2 u -- )

\ cell size and storage primitives - extensions
\ FIXME - doesn't work if characters need alignment
: ERASE ( addr u -- ) CHARS 0 FILL ;
