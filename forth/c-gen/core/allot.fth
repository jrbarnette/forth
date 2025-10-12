\  Copyright 2023, by J. Richard Barnette. All Rights Reserved.

\ arith operations
: +! ( x a-addr -- ) dup >r @ + r> ! ;
: WITHIN ( x1 x2 x3 -- flag ) over - >r - r> u< ;

\ dictionary operations
: HERE <C> &HERE; @ ;

: UNUSED ( -- u ) <C> DICTSPACE_END; here - ;
: allot-bounds ( -- unused+1 -inuse )
    unused 1 + dup <C> DICTSPACE_SIZE+1; - ;

: ALLOT ( n -- )
    dup allot-bounds within -8 and throw <C> &HERE; +! ;
: ALIGN ( -- ) here aligned <C> &HERE; ! ;
: , ( x -- ) here <C> CELL_SIZE; allot ! ;
: C, ( char -- ) here <C> CHAR_SIZE; allot c! ;
: COMPILE, ( xt -- ) , ; compile-only
