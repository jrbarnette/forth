\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.

\ XXX - this doesn't belong here, but it can't go into the compiled
\ dictionary, because the meta-interpreter won't handle postpone s"
\ properly.
: ." postpone s" postpone type ; immediate

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
