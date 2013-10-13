\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\ !                     6.1.0010 CORE                   25
\ +!                    6.1.0130 CORE                   27
\ 2!                    6.1.0310 CORE                   29
\ 2@                    6.1.0350 CORE                   29
\ @                     6.1.0650 CORE                   32
\ ALIGNED               6.1.0706 CORE                   33
\ C!                    6.1.0850 CORE                   34
\ C@                    6.1.0870 CORE                   34
\ CELL+                 6.1.0880 CORE                   35
\ CELLS                 6.1.0890 CORE                   35
\ CHAR+                 6.1.0897 CORE                   35
\ CHARS                 6.1.0898 CORE                   35
\ COUNT                 6.1.0980 CORE                   36
\ FILL                  6.1.1540 CORE                   39
\ MOVE                  6.1.1900 CORE                   42
\ ------  ------  ------  ------  ------  ------  ------  ------

\ primitives
\ ! ( x a-addr -- )
\ @ ( a-addr -- x )
\ C! ( char c-addr -- )
\ C@ ( c-addr -- char ) 

\ primitives - could be in forth
\ FILL ( c-addr u char -- )
\ MOVE ( addr1 addr2 u -- )

\ non-primitive - target cpu specific
\ assumes byte-addressable, 1-byte characters
: CHARS ( n1 -- n2 ) ;

\ 16-, 32-, and 64-bit versions
\ : CELLS ( n1 -- n2 ) 1 lshift ;
: CELLS ( n1 -- n2 ) 2 lshift ;
\ : CELLS ( n1 -- n2 ) 3 lshift ;

: CELL+ ( a-addr1 -- a-addr2 ) [ 1 cells ] literal + ;
: ALIGNED ( addr -- a-addr )
    [ 1 cells dup 1- ] literal + [ negate ] literal and ;
: CHAR+ ( c-addr1 -- c-addr2 ) [ 1 chars ] literal + ;
: +! ( x a-addr -- ) dup >r @ + r> ! ;
: 2! ( x1 x2 a-addr -- ) swap over ! cell+ ! ;
: 2@ ( a-addr -- x1 x2 ) dup cell+ @ swap @ ;
: COUNT ( c-addr -- c-addr u ) dup char+ swap c@ ;


\ ------  ------  ------  ------  ------  ------  ------  ------
\ ERASE                 6.2.1350 CORE EXT               53
\ ------  ------  ------  ------  ------  ------  ------  ------

\ FIXME - doesn't work if characters need alignment
: ERASE ( addr u -- ) CHARS 0 FILL ;
