\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  GET-ORDER          16.6.1.1647 SEARCH
\  SET-ORDER          16.6.1.2197 SEARCH
\ ------  ------  ------  ------  ------  ------  ------  ------

\ layout of dictionary for name space variables:
\   forth-wordlist @            -> latest definition in FORTH
\   forth-wordlist cell+        -> pointer to CURRENT wordlist
\   forth-wordlist 2 cells + @  -> # wordlists in search order
\   forth-wordlist 3 cells +    -> first entry in search order

: GET-ORDER ( -- widn ... wid1 n )
    [ forth-wordlist 2 cells + ] literal
    dup @ dup >r swap over cells + swap
    begin dup while
	>r dup @ swap [ 1 cells ] literal - r> 1-
    repeat 2drop r>
;
: SET-ORDER ( widn ... wid1 n -- )
    dup -1 = if drop forth-wordlist 1 then
    [ forth-wordlist 2 cells + ] literal
    2dup ! swap				( wids ptr n )
    begin dup while			( wids ptr n )
	>r cell+ swap over ! r> 1-	( wids' ptr+ n- )
    repeat 2drop
;
