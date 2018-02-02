\ Copyright 2018, by J. Richard Barnette, All Rights Reserved.
\ Internal definitions relating to namespace.

: toupper ( char -- char )
    dup [char] a - 26 u< if [ char A char a - ] literal + then
;

base @ hex
80 constant NF-IMMEDIATE
40 constant NF-COMPILE-ONLY
1f constant NF-LENGTH
nf-length ff xor constant NF-FLAGS

: name>id ( name -- c-addr u ) cell+ count nf-length and ;
: name>xt ( name -- xt flags )
    cell+ count dup nf-flags and >r nf-length and chars + aligned r> ;
base !

: flags>find ( flags -- -1 | 1 ) nf-immediate and 0= 1 or ;
: flags! ( flags wid -- ) @ cell+ dup >r c@ or r> c! ;

: id, ( c-addr u -- )
    dup c, here swap chars dup allot move
    \ 0 do over i chars + c@ toupper over i chars + c! loop
;

: name, ( mcp wid "<spaces>name<space>" -- name )
    align here >r @ , parse-name id, align , r>
;

: id= ( c-addr1 u1 c-addr2 u2 -- flag )
    rot over <> if drop 2drop false exit then
    0 do
	over i + c@ toupper over i + c@ toupper <> if
	    2drop false unloop exit
	then
    [ 1 chars ] literal +loop 2drop true
;

: wid-lookup ( c-addr u wid -- xt flags -1 | 0 )
    begin @ dup while
	>r 2dup r@ name>id id= if 2drop r> name>xt true exit then r>
    repeat drop 2drop false
;

: lookup ( wid ... n c-addr u -- xt flags -1 | 0 )
    2>r begin dup while 1- swap
	2r@ rot wid-lookup if
	    2>r begin dup while 1- swap drop repeat drop 2r> true
	    2r> 2drop exit
	then
    repeat 2r> 2drop
;
