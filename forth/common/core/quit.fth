\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  ABORT                 6.1.0670 CORE
\  ABORT"                6.1.0680 CORE
\  ENVIRONMENT?          6.1.1345 CORE
\  EVALUATE              6.1.1360 CORE
\  QUIT                  6.1.2050 CORE
\ ------  ------  ------  ------  ------  ------  ------  ------

: EVALUATE			( i*x c-addr u -- j*x )
    \ save current source specification
    >in @ source-id 2>r source 2>r
    \ make the string the input source
    source! -1 source-id! 0 >in ! interpret
    \ restore original source specification
    2r> source! 2r> source-id! >in !
;

here 256 chars allot    \ constant TIB
: QUIT
    rclear
    [ swap ] literal 256 source! 0 source-id! 0 >in !
    postpone [
    begin state @ if s" ok] " else s" ok[ " then prompt refill while
        interpret
    repeat cr ." bye" cr 0 >r
;

: ABORT clear quit ;
: ABORT"
    postpone if postpone ." postpone cr postpone abort postpone then
; compile-only

\  ENVIRONMENT? - TBD
