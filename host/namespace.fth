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
: name>find ( name -- xt 1 | xt -1 )
    name>xt nf-immediate and if 1 else -1 then ;
base !

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
	    unloop 2drop false exit
	then
    [ 1 chars ] literal +loop 2drop true
;

: lookup ( c-addr u wid -- xt flags -1 | 0 )
    begin @ dup while
	>r 2dup r@ name>id id= if 2drop r> name>xt true exit then r>
    repeat >r 2drop r>
;
