\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\  +!                    6.1.0130 CORE                   27
\  2!                    6.1.0310 CORE                   29
\  2@                    6.1.0350 CORE                   29
\  ALIGNED               6.1.0706 CORE                   33
\  COUNT                 6.1.0980 CORE                   36
\ ------  ------  ------  ------  ------  ------  ------  ------

: +! ( x a-addr -- ) dup >r @ + r> ! ;
: 2! ( x1 x2 a-addr -- ) swap over ! cell+ ! ;
: 2@ ( a-addr -- x1 x2 ) dup cell+ @ swap @ ;
: ALIGNED ( addr -- a-addr )
    [ 1 cells 1 - ] literal + [ -1 cells ] literal and ;
: COUNT ( c-addr1 -- c-addr2 u ) dup char+ swap c@ ;


\ ------  ------  ------  ------  ------  ------  ------  ------
\  ERASE                 6.2.1350 CORE EXT               53
\ ------  ------  ------  ------  ------  ------  ------  ------

\ FIXME - doesn't work if characters need alignment
: ERASE ( addr u -- ) CHARS 0 FILL ;
