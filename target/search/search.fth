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

\ FORTH-WORDLIST ( -- wid )
\ GET-CURRENT ( -- wid )
\ SET-CURRENT ( wid -- )
\ GET-ORDER ( -- widn ... wid1 n )
\ SET-ORDER ( widn ... wid1 n -- )
\ SEARCH-WORDLIST ( c-addr u -- 0 | xt 1 | xt -1 )
\ anonymous
: drop-order ( widn ... wid1 n -- ) begin dup while nip 1- repeat drop ;
: DEFINITIONS ( -- ) get-order swap set-current 1- drop-order ;
: WORDLIST ( -- wid ) here 0 , ;

: FIND ( c-addr -- c-addr 0 | xt 1 | xt -1 )
    >r get-order begin dup while		( wid ... n )
	1- swap r@ count rot search-wordlist	( wid ... n search )
	?dup if 2>r drop-order 2r> r> drop exit then
    repeat drop r> 0
;

\ ------  ------  ------  ------  ------  ------  ------  ------
\ ALSO               16.6.2.0715 SEARCH EXT            120
\ FORTH              16.6.2.1590 SEARCH EXT            120
\ ONLY               16.6.2.1965 SEARCH EXT            121
\ ORDER              16.6.2.1985 SEARCH EXT            121
\ PREVIOUS           16.6.2.2037 SEARCH EXT            121
\ ------  ------  ------  ------  ------  ------  ------  ------

: ALSO ( -- ) get-order over swap 1+ set-order ;
: FORTH ( -- ) get-order nip forth-wordlist swap set-order ;
: ONLY ( -- ) -1 set-order ;
: PREVIOUS ( -- ) get-order nip 1- set-order ;
