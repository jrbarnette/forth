\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.

\ XXX - this doesn't belong here, but it can't go into the compiled
\ dictionary, because the meta-interpreter won't handle postpone s"
\ properly.
: ." postpone s" postpone type ; immediate

here 256 dup chars allot        \ constant #TIB constant TIB
( C: TIB #TIB )
: QUIT
    rclear [ rot ] literal [ swap ] literal source<terminal
    \ For now, REFILL does the "OK" prompt.  That's (mostly) wrong.
    postpone [
    begin refill while interpret repeat 0 >r
;

: ABORT clear quit ;
: ABORT" postpone if postpone ." postpone abort postpone then ; compile-only
