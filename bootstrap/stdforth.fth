\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.

: [CHAR] char postpone literal ; compile-only
: ['] ' postpone literal ; compile-only


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

\ terminal I/O - CORE
: CR ( -- ) 10 emit ;
: SPACE ( -- ) bl emit ;
: SPACES ( n -- ) begin dup 0> while space 1- repeat drop ;
: TYPE ( c-addr u -- )
    chars over + swap begin 2dup > while dup c@ emit char+ repeat 2drop
;

: . <# bl hold dup abs 0 #s rot sign #> type ;
: U. <# bl hold 0 #s #> type ;
: .R ( n1 n2 -- ) >r <# dup abs 0 #s rot sign #> r> over - spaces type ;
: U.R ( u n -- ) >r <# 0 #s #> r> over - spaces type ;

: ." postpone s" postpone type ; immediate

: .( [char] ) parse type ; immediate
: ABORT"
    postpone if postpone ." postpone abort postpone then
; immediate
