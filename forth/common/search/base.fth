\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  DEFINITIONS        16.6.1.1180 SEARCH
\  GET-ORDER          16.6.1.1647 SEARCH
\  SET-ORDER          16.6.1.2197 SEARCH
\ ------  ------  ------  ------  ------  ------  ------  ------

\ Search order:  1 cell for length of search order; 8 cells for
\ the wordlists to be searched.

variable SEARCH-ORDER 8 cells allot

: GET-ORDER ( -- widn ... wid1 n )
    search-order dup @ dup >r cells + r>
    ( so-end so# ) ( R: so# )
    begin dup while
	>r dup @ swap [ 1 cells ] literal - r> 1-
    repeat drop @
;
: SET-ORDER ( widn ... wid1 n -- )
    dup 8 > if -49 throw then
    dup -1 = if drop forth-wordlist 1 then
    search-order 2dup ! swap		( wids ptr n )
    begin dup while			( wids ptr n )
	>r cell+ swap over ! r> 1-	( wids' ptr+ n- )
    repeat 2drop
;

: CONTEXT ( -- a-addr )
    search-order dup @ if cell+ else -50 throw then ;

: DEFINITIONS ( -- ) context @ set-current ;

forth-wordlist 1 set-order definitions
