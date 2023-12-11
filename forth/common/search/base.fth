\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  DEFINITIONS        16.6.1.1180 SEARCH
\  GET-ORDER          16.6.1.1647 SEARCH
\  SET-ORDER          16.6.1.2197 SEARCH
\ ------  ------  ------  ------  ------  ------  ------  ------

\ Search order:  1 cell for length of search order; 8 cells for
\ the wordlists to be searched.

here 9 cells allot		( search-order )

: GET-ORDER ( -- widn ... wid1 n )
    [ over ] literal dup @ dup >r cells + r@
    begin dup while
	>r dup @ swap [ 1 cells ] literal - r> 1-
    repeat 2drop r>
;
: SET-ORDER ( widn ... wid1 n -- )
    dup 8 > if -49 throw then
    dup -1 = if drop forth-wordlist 1 then
    [ over ] literal 2dup ! swap	( wids ptr n )
    begin dup while			( wids ptr n )
	>r cell+ swap over ! r> 1-	( wids' ptr+ n- )
    repeat 2drop
;

: get-order-first-entry ( -- a-addr )
    [ swap dup ] literal @ if [ swap cell+ ] literal else -50 throw then ;

: DEFINITIONS ( -- ) get-order-first-entry @ set-current ;

forth-wordlist 1 set-order definitions
