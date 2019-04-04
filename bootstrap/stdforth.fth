\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.

\ SEARCH
\ layout of dictionary for name space variables:
\   forth-wordlist @            -> latest definition in FORTH
\   forth-wordlist cell+        -> pointer to CURRENT wordlist
\   forth-wordlist 2 cells + @  -> # wordlists in search order
\   forth-wordlist 3 cells +    -> first entry in search order

: WORDLIST ( -- wid ) here 0 , ;

\ NO-NAME code: make TOS the first wordlist in search order
here forth-wordlist 3 cells + ' definitions @ , ] literal ! exit [
: VOCABULARY create 0 , does> [ ( C: xt colon-sys ) over , ] ;
: FORTH ( -- ) forth-wordlist [ ( C: xt colon-sys ) swap , ] ;

: ALSO ( -- ) get-order over swap 1+ set-order ;
: ONLY ( -- ) -1 set-order ;
: PREVIOUS ( -- ) get-order nip 1- set-order ;

: ." postpone s" postpone type ; immediate

: EVALUATE			( i*x c-addr u -- j*x )
    \ save current source specification
    >in @ source-id 2>r source 2>r
    \ make the string the input source
    source! -1 source-id! 0 >in ! interpret
    \ restore original source specification
    2r> source! 2r> source-id! >in !
;

here 256 dup chars allot    \ constant TIB
: QUIT
    rclear
    \ make the terminal buffer the input source
    [ rot ] literal [ swap ] literal source! 0 source-id! 0 >in !
    postpone [
    \ No prompt just yet: we need to know whether this is
    \ interactive, and we need an interface into the C readline()
    \ library.
    begin refill while interpret repeat 0 >r
;

: ABORT clear quit ;
: ABORT" postpone if postpone ." postpone abort postpone then ; compile-only
