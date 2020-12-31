\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  ABORT                 6.1.0670 CORE
\  ABORT"                6.1.0680 CORE
\  QUIT                  6.1.2050 CORE
\ ------  ------  ------  ------  ------  ------  ------  ------

: state-prompt state @ if s" " else s" ok " then prompt! ;
: QUIT
    rclear 0 source-id! postpone [
    begin state-prompt refill while interpret repeat
;

: ABORT clear quit ;
: ABORT" postpone if postpone ." postpone abort postpone then ; compile-special
