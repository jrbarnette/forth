\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.

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
