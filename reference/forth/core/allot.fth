\ arith primitives ...
\ mult primitives ...
\ stack primitives ...
\ stack operations ...
\ memory primitives ...

\ memory operations
: CHARS ;
: CHAR+ 1 + ;

: CELLS <C> CELL_SIZE; * ;
: CELL+ <C> CELL_SIZE; + ;
: ALIGNED ( addr -- a-addr ) <C> CELL_SIZE-1; + <C> -CELL_SIZE; and ;

: 2! ( x1 x2 a-addr -- ) swap over ! cell+ ! ;
: 2@ ( a-addr -- x1 x2 ) dup cell+ @ swap @ ;
: COUNT ( c-addr1 -- c-addr2 u ) dup char+ swap c@ ;
: +! ( x a-addr -- ) dup >r @ + r> ! ;

\ arith operations
: WITHIN ( x1 x2 x3 -- flag ) over - >r - r> u< ;

\ dictionary operations
0 constant HERE

: allot-bounds ( -- -inuse unused+1 )
    <C> DICTSPACE_END+1; here - dup <C> DICTSPACE_SIZE+1; - ;
: ALLOT ( n -- )
    dup allot-bounds within -8 and throw [ ' here cell+ ] literal +! ;
: ALIGN ( -- ) [ 1 cells 1- ] literal dup here + and allot ;
\ : ALIGN here aligned [ ' here cell+ ] literal ! ;

: , ( x -- ) here [ 1 cells ] literal allot ! ;
: C, ( char -- ) here [ 1 chars ] literal allot c! ;
: COMPILE, ( xt -- ) , ;

: UNUSED ( -- u ) <C> DICTSPACE_END; here - ;

: LITERAL do-literal do-literal compile, , ; COMPILE-SPECIAL
