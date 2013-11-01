\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\ #                     6.1.0030 CORE                   25
\ #>                    6.1.0040 CORE                   25
\ #S                    6.1.0050 CORE                   25
\ .                     6.1.0180 CORE                   27
\ <#                    6.1.0490 CORE                   31
\ >NUMBER               6.1.0570 CORE                   31
\ BASE                  6.1.0750 CORE                   34
\ DECIMAL               6.1.1170 CORE                   36
\ HOLD                  6.1.1670 CORE                   39
\ SIGN                  6.1.2210 CORE                   45
\ U.                    6.1.2320 CORE                   46
\ ------  ------  ------  ------  ------  ------  ------  ------

variable BASE
: DECIMAL [ 1 dup + dup + 1 + dup + ] literal base ! ;
decimal

variable hold-count
1 cells 8 * 2 * 2 + 2 cells + allot
here constant hold-end
: hold-pointer ( -- c-addr ) hold-end hold-count @ chars - ;

: <# ( -- ) 0 hold-count ! ;
: HOLD ( char -- ) 1 hold-count +! hold-pointer c! ;
: #> ( xd -- c-addr u ) 2drop hold-pointer hold-count @ ;

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


\ ------  ------  ------  ------  ------  ------  ------  ------
\ .R                    6.2.0210 CORE EXT               49
\ HEX                   6.2.1660 CORE EXT               54
\ U.R                   6.2.2330 CORE EXT               57
\ ------  ------  ------  ------  ------  ------  ------  ------

BASE @ DECIMAL : HEX 16 BASE ! ; BASE !
