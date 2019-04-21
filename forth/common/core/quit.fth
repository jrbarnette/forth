\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  ABORT                 6.1.0670 CORE
\  ABORT"                6.1.0680 CORE
\  QUIT                  6.1.2050 CORE
\ ------  ------  ------  ------  ------  ------  ------  ------

( C: TIB #TIB )
: QUIT
    rclear source<terminal postpone [
    begin state @ if s" " else s" ok " then prompt!
	refill while interpret repeat 0 >r
;

: ABORT clear quit ;
: ABORT" postpone if postpone ." postpone abort postpone then ; compile-special
