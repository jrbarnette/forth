\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.

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
