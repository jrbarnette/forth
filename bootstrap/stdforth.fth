char = parse Copyright 2013, by J. Richard Barnette, All Rights Reserved. =
drop drop

: \ source >in ! drop ; immediate
\ Whew! Now we can write real comments!

: [CHAR] char postpone literal ; immediate \ no-interpret
: ( [char] ) parse drop drop ; immediate
: ['] ' postpone literal ; immediate \ no-interpret

32 constant BL
0 0 = constant TRUE
1 0 = constant FALSE


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

\ memops - CORE
: CHARS ( n1 -- n2 ) ;
: CELLS ( n1 -- n2 ) 3 lshift ;
: CELL+ ( a-addr1 -- a-addr2 ) [ 1 cells ] literal + ;
: ALIGNED ( addr -- a-addr )
    [ 1 cells 1- ] literal + [ -1 cells ] literal and ;
: CHAR+ ( c-addr1 -- c-addr2 ) [ 1 chars ] literal + ;
: +! ( x a-addr -- ) dup >r @ + r> ! ;
: 2! ( x1 x2 a-addr -- ) swap over ! cell+ ! ;
: 2@ ( a-addr -- x1 x2 ) dup cell+ @ swap @ ;
: COUNT ( c-addr -- c-addr u ) dup char+ swap c@ ;


\ compile - CORE
: COMPILE, ( xt -- ) , ; \ no-interpret
: LOOP 1 postpone literal postpone +loop ; immediate \ no-interpret


: FIND ( c-addr -- c-addr 0 | xt 1 | xt -1 )
    dup >r count forth-wordlist search-wordlist r>
    over if drop else swap then
;

\ pictured string formatting - CORE
1 cells 8 * 2 * 2 + chars allot align
here 1 cells allot			( hold-addr )
: <# ( -- ) 0 [ over ] literal ! ;
: HOLD ( char -- ) [ over ] literal dup @ 1+ 2dup swap ! chars - c! ;
: #> ( xd -- c-addr u ) 2drop [ over ] literal dup @ dup >r chars - r> ;
drop					( )

: SIGN ( n -- ) 0< if [char] - hold then ;
: # ( ud1 -- ud2 )
    0 base @ um/mod rot swap base @ um/mod
    dup 10 u< if [char] 0 else [ char A 10 - ] literal then
    + hold swap
;
: #S ( ud1 -- ud2 ) begin # 2dup or 0= until ;

: >NUMBER ( ud1 c-addr1 u1 -- ud2 c-addr2 u2 )
    begin dup while			( ud c-addr u )
	>r dup >r c@			( ud char ) ( R: u c-addr )
	[char] 0 -
	9 over u< if [ char 0 char A - ] literal + then
	dup base @ u< invert if drop r> r> exit then
	>r base @ * >r base @ um* r> + r>
	rot dup >r + dup r> u< negate rot +
	r> char+ r> 1-
    repeat
;

\ terminal I/O - CORE
: CR ( -- ) 10 emit ;
: SPACE ( -- ) bl emit ;
: SPACES ( n -- ) dup 0> if 0 do space loop else drop then ;
: TYPE ( c-addr u -- )
    \ missing ?do :-(
    dup 0= if 2drop exit then
    chars over + swap do i c@ emit [ 1 chars ] literal +loop
;

: . <# bl hold dup abs 0 #s rot sign #> type ;
: U. <# bl hold 0 #s #> type ;
: .R ( n1 n2 -- ) >r <# dup abs 0 #s rot sign #> r> over - spaces type ;
: U.R ( u n -- ) >r <# 0 #s #> r> over - spaces type ;

: ." postpone s" postpone type ; immediate

: .( [char] ) parse type ; immediate
: ABORT"
    postpone if postpone ." postpone abort postpone then
; immediate

unused				( free )
here forth-wordlist - cell+	( free inuse )
2dup + rot rot			( total free inuse )
.( dictionary: ) . .( in use / ) . .( free / ) . .( total ) cr
