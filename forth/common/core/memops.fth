\ Copyright 2020, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\  2!                    6.1.0310 CORE
\  2@                    6.1.0350 CORE
\  ALIGNED               6.1.0706 CORE
\  COUNT                 6.1.0980 CORE
\
\  ERASE                 6.2.1350 CORE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------

: 2! ( x1 x2 a-addr -- ) swap over ! cell+ ! ;
: 2@ ( a-addr -- x1 x2 ) dup cell+ @ swap @ ;
: ALIGNED ( addr -- a-addr )
    [ 1 cells 1 - ] literal + [ -1 cells ] literal and ;
: COUNT ( c-addr1 -- c-addr2 u ) dup char+ swap c@ ;

\ FIXME - doesn't work if characters need alignment
: ERASE ( addr u -- ) CHARS 0 FILL ;
