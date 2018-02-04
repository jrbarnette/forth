\ Copyright 2018, by J. Richard Barnette, All Rights Reserved.
\ Internal definitions relating to namespace.

base @ hex
40 constant NF-COMPILE-ONLY
80 constant NF-IMMEDIATE
c0 constant NF-COMPILE-SPECIAL
1f constant NF-LENGTH
e0 constant NF-FLAGS
base !

: NAME>ID ( name -- c-addr u ) cell+ count nf-length and ;
: NAME>XT ( name -- xt ) name>id chars + aligned ;
: NAME>XT+FLAGS ( name -- xt flags )
    cell+ count dup >r nf-length and chars + aligned r> nf-flags and ;
: NAME>FIND ( name -- xt -1 | xt 1 | 0 )
    dup if name>xt+flags nf-immediate and 0= 1 or then ;
: FLAGS! ( flags wid -- ) @ cell+ dup >r c@ or r> c! ;

: ID, ( c-addr u -- )
    dup c, here swap chars dup allot move
    \ 0 do over i chars + c@ toupper over i chars + c! loop
;

: NAME, ( mcp wid "<spaces>name<space>" -- name )
    align here >r @ , parse-name id, align , r>
;

: ID= ( c-addr1 u1 c-addr2 u2 -- flag )
    rot over <> if drop 2drop false exit then
    0 do
	over i + c@ toupper over i + c@ toupper <> if
	    2drop false unloop exit
	then
    [ 1 chars ] literal +loop 2drop true
;

: WID-LOOKUP ( c-addr u wid -- name | 0 )
    begin @ dup while >r
	2dup r@ name>id id= if 2drop r> exit then
    r> repeat drop 2drop 0
;

: LOOKUP ( wid ... n c-addr u -- name | 0 )
    2>r begin dup while 1- swap
	2r@ rot wid-lookup ?dup if >r
	    begin dup while 1- swap drop repeat drop
	    r> 2r> 2drop exit
	then
    repeat 2r> 2drop
;
