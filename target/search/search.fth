\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\ DEFINITIONS        16.6.1.1180 SEARCH                119
\ FIND               16.6.1.1550 SEARCH                119
\ FORTH-WORDLIST     16.6.1.1595 SEARCH                119
\ GET-CURRENT        16.6.1.1643 SEARCH                119
\ GET-ORDER          16.6.1.1647 SEARCH                119
\ SEARCH-WORDLIST    16.6.1.2192 SEARCH                120
\ SET-CURRENT        16.6.1.2195 SEARCH                120
\ SET-ORDER          16.6.1.2197 SEARCH                120
\ WORDLIST           16.6.1.2460 SEARCH                120
\ ------  ------  ------  ------  ------  ------  ------  ------

variable CURRENT
: GET-CURRENT ( -- wid ) current @ ;
: SET-CURRENT ( wid -- ) current ! ;

\ non-standard definitions
base @ hex
80 constant NF-IMMEDIATE
40 constant NF-NO-INTERPRET
nf-immediate nf-no-interpret or
    constant NF-COMPILE-ONLY
: >XT ( name -- xt ) cell+ dup c@ 1f and 1+ chars + aligned ;
: >FLAGS ( name -- flags ) cell+ dup c@ c0 and ;
: >ID ( name -- c-addr u ) cell+ count 1f and ;
base !

: SET-FLAGS ( flags -- ) get-current @ cell+ dup >r c@ or r> c! ;
: NO-INTERPRET nf-no-interpret set-flags ;
: COMPILE-ONLY nf-compile-only set-flags ;

\ standard
: IMMEDIATE nf-immediate set-flags ;

\ non-standard
variable ORDER 8 cells allot
: GET-ORDER ( -- widn ... wid1 n )
    order dup @ >r
    swap over cells + swap begin dup while
	>r dup @ swap [ 1 cells ] literal - r> 1-
    repeat 2drop r>
;
: SET-ORDER ( widn ... wid1 n -- )
    dup -1 = if drop forth-wordlist 1 then
    order 2dup ! swap begin dup while
	>r cell+ swap over ! r> 1-
    repeat drop
;

\ non-standard definitions
: TOUPPER ( char -- char )
    dup [char] a - 26 u< if [ char A char a - ] literal + then
;
: COMPARE-IDS ( c-addr1 u1 c-addr2 u2 -- c-addr1 u1 <>? )
    swap >r over = if			( c-addr1 u1 ) ( R: c-addr2 )
	2dup r> swap begin dup while	( c-addr1 c-addr2 u )
	    >r over c@ toupper over c@ toupper = if
		r> drop 2drop false exit
	    then
	    char+ >r char+ r> r> 1-
	repeat drop 2drop
    else			( c-addr1 u1 ) ( R: c-addr2 )
	r> drop
    then true
;
base @ hex
: LOOKUP ( c-addr u wid -- name | 0 )
    @ dup begin dup while			( c-addr u name name )
	>r cell+ count 1f and
	compare-ids r> swap			( c-addr u name flag )
	if @ dup else false then
    repeat drop >r 2drop r>
;
base !

: >FIND ( name -- xt 1 | xt -1 )
    dup >r >xt -1 r> >flags nf-immediate and if negate then
;
: SEARCH-ORDER ( c-addr u -- name | 0 )
    2>r order dup @ swap over begin while
	cell+ swap over @ 2r@ rot lookup ?dup if
	    swap drop swap 0
	else
	    1- swap over
	then
    repeat drop 2r> 2drop
;

\ standard
: SEARCH-WORDLIST ( c-addr u wid -- 0 | xt 1 | xt -1 )
    lookup dup if >find then
;
: FIND ( c-addr -- c-addr 0 | xt 1 | xt -1 )
    dup count search-order dup if swap drop >find then
;

: WORDLIST ( -- wid ) here 0 , ;
wordlist constant FORTH-WORDLIST
: DEFINITIONS ( -- ) order cell+ @ set-current ;


\ ------  ------  ------  ------  ------  ------  ------  ------
\ ALSO               16.6.2.0715 SEARCH EXT            120
\ FORTH              16.6.2.1590 SEARCH EXT            120
\ ONLY               16.6.2.1965 SEARCH EXT            121
\ ORDER              16.6.2.1985 SEARCH EXT            121
\ PREVIOUS           16.6.2.2037 SEARCH EXT            121
\ ------  ------  ------  ------  ------  ------  ------  ------

: ALSO ( -- ) get-order over swap 1+ set-order ;
: FORTH ( -- ) get-order >r drop forth-wordlist r> set-order ;
: ONLY ( -- ) -1 set-order ;
: PREVIOUS ( -- ) get-order nip 1- set-order ;
