\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.

\ compile - CORE EXT
: COMPILE, ( xt -- ) , ; no-interpret

\ parse - CORE EXT (2012 standard)
: PARSE-NAME
    source swap >r >in @ 1-                     ( end idx ) ( R: c-addr )
    begin 1+ 2dup > while			( end idx )
	r@ over chars + c@ 33 127 within
    until then >in ! drop r> drop bl parse
;

\ SEARCH
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
: DEFINITIONS ( -- ) [ forth-wordlist 3 cells + ] literal @ set-current ;
: WORDLIST ( -- wid ) here 0 , ;

\ NO-NAME code: make TOS the first wordlist in search order
here forth-wordlist 3 cells + ' definitions @ , ] literal ! exit [
: VOCABULARY create 0 , does> [ ( C: xt colon-sys ) over , ] ;
: FORTH ( -- ) forth-wordlist [ ( C: xt colon-sys ) swap , ] ;

: ALSO ( -- ) get-order over swap 1+ set-order ;
: ONLY ( -- ) -1 set-order ;
: PREVIOUS ( -- ) get-order nip 1- set-order ;

: ." postpone s" postpone type ; immediate

: ABORT"
    postpone if postpone ." postpone abort postpone then
; immediate
