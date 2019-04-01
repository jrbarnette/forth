\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  2DROP                 6.1.0370 CORE                   29
\  2DUP                  6.1.0380 CORE                   29
\  2OVER                 6.1.0400 CORE                   29
\  2SWAP                 6.1.0430 CORE                   30
\
\  NIP                   6.2.1930 CORE EXT               54
\  TUCK                  6.2.2300 CORE EXT               57
\ ------  ------  ------  ------  ------  ------  ------  ------

: 2DROP ( x1 x2 -- ) drop drop ;
: 2DUP ( x1 x2 -- x1 x2 x1 x2 ) over over ;
: 2SWAP ( x1 x2 x3 x4 -- x3 x4 x1 x2 ) rot >r rot r> ;
: 2OVER ( x1 x2 x3 x4 -- x1 x2 x3 x4 x1 x2 ) >r >r 2dup r> r> 2swap ;


: NIP ( x1 x2 -- x2 ) swap drop ;
: TUCK ( x1 x2 -- x2 x1 x2 ) swap over ;
