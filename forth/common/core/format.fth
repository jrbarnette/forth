\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  #                     6.1.0030 CORE                   25
\  #>                    6.1.0040 CORE                   25
\  #S                    6.1.0050 CORE                   25
\  <#                    6.1.0490 CORE                   31
\  >NUMBER               6.1.0570 CORE                   31
\  HOLD                  6.1.1670 CORE                   39
\  SIGN                  6.1.2210 CORE                   45
\ ------  ------  ------  ------  ------  ------  ------  ------

1 cells 8 * 2 * 2 + chars allot align
here 1 cells allot			( hold-addr )
: <# ( -- ) 0 [ over ] literal ! ;
: HOLD ( char -- ) [ over ] literal dup @ 1+ 2dup swap ! chars - c! ;
: #> ( xd -- c-addr u ) 2drop [ over ] literal dup @ dup >r chars - r> ;
drop					( )

: # ( ud1 -- ud2 )
    0 base @ um/mod >r base @ um/mod swap
    \ XXX meta-interpretation can't handle `char`
    dup 10 u< if [char] 0 else [ char A 10 - ] literal then
    \ dup 10 u< if [char] 0 else [char] A 10 - then
    + hold r>
;
: #S ( ud1 -- ud2 ) begin # 2dup or 0= until ;
: SIGN ( n -- ) 0< if [char] - hold then ;

: >DIGIT ( char -- u )
    [char] 0 - 9 over u< if
	17 - 25 over u< if 32 - 25 over u< if drop -1 exit then then 10 +
    then
;

: >NUMBER ( ud1 c-addr1 u1 -- ud2 c-addr2 u2 )
    begin dup while			( ud c-addr u )
	>r dup >r c@ >digit		( ud digit ) ( R: u c-addr )
	dup base @ u< invert if drop r> r> exit then
	>r base @ * >r base @ um* r> + r>
	rot dup >r + dup r> u< negate rot +
	r> char+ r> 1-
    repeat
;

: . <# bl hold dup abs 0 #s rot sign #> type ;
: ." postpone s" postpone type ; compile-only
: U. <# bl hold 0 #s #> type ;

: .R ( n1 n2 -- ) >r <# dup abs 0 #s rot sign #> r> over - spaces type ;
: U.R ( u n -- ) >r <# 0 #s #> r> over - spaces type ;
