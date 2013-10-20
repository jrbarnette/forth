char = parse Copyright 2013, by J. Richard Barnette, All Rights Reserved. =
drop drop

: \ source >in ! drop ; immediate
\ Whew! Glad that's taken care of!

: [CHAR] char postpone literal ; immediate
: ( [char] ) parse drop drop ; immediate

32 constant BL
0 0 = constant TRUE
1 0 = constant FALSE


: LOOP 1 postpone literal postpone +loop ; immediate
: COMPILE, ( xt -- ) , ;


\ stackops - CORE
: 2DROP ( x1 x2 -- ) drop drop ;
: 2DUP ( x1 x2 -- x1 x2 x1 x2 ) over over ;
: 2SWAP ( x1 x2 x3 x4 -- x3 x4 x1 x2 ) rot >r rot r> ;
: 2OVER ( x1 x2 x3 x4 -- x1 x2 x3 x4 x1 x2 ) >r >r 2dup r> r> 2swap ;

\ stackops - CORE EXT
: NIP ( x1 x2 -- x2 ) swap drop ;
: TUCK ( x1 x2 -- x2 x1 x2 ) swap over ;

\ arithops - CORE
: 0< ( n -- flag ) 0 < ;
: 0= ( n -- flag ) 0 = ;
: 1+ ( x1 -- x2 ) 1 + ;
: 1- ( x1 -- x2 ) 1 - ;
: ABS ( n -- u ) dup 0< if negate then ;
: MAX ( n1 n2 -- n3 ) 2dup < if swap then drop ;
: MIN ( n1 n2 -- n3 ) 2dup > if swap then drop ;
: S>D ( n -- d ) dup 0< ;

\ arithops - CORE EXT
: <> ( n1 n2 -- flag ) = invert ;
: 0<> ( n -- flag ) 0 <> ;
: 0> ( n -- flag ) 0 > ;
: U> ( n1 n2 -- flag ) swap u< ;
: WITHIN ( x1 x2 x3 -- flag ) over - >r - r> u< ;

: CHARS ( n1 -- n2 ) ;
: CELLS ( n1 -- n2 ) 3 lshift ;
: CELL+ ( a-addr1 -- a-addr2 ) [ 1 cells ] literal + ;
: ALIGNED ( addr -- a-addr )
    [ 1 cells dup 1- ] literal + [ negate ] literal and ;
: CHAR+ ( c-addr1 -- c-addr2 ) [ 1 chars ] literal + ;
: +! ( x a-addr -- ) dup >r @ + r> ! ;
: 2! ( x1 x2 a-addr -- ) swap over ! cell+ ! ;
: 2@ ( a-addr -- x1 x2 ) dup cell+ @ swap @ ;
: COUNT ( c-addr -- c-addr u ) dup char+ swap c@ ;


: FIND ( c-addr -- c-addr 0 | xt 1 | xt -1 )
    dup >r count forth-wordlist search-wordlist r>
    over if drop else swap then
;
: ['] ( "<spaces>name" -- ) ' postpone literal ; immediate \ no-interpret


: CR ( -- ) 10 emit ;
: SPACE ( -- ) bl emit ;
: SPACES ( n -- ) 0 ?do space loop ;
: TYPE ( c-addr u -- )
    chars over + swap ?do i c@ emit [ 1 chars ] literal +loop
;
: ." postpone s" type ; immediate

: .( [char] ) parse type ; immediate
: ABORT"
    postpone if postpone ." postpone abort postpone then
; immediate
