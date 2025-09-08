\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  INCLUDE-FILE       11.6.1.1717 FILE
\  INCLUDED           11.6.1.1718 FILE
\ ------  ------  ------  ------  ------  ------  ------  ------

: INTERPRET-SOURCE
    >r begin refill if r@ catch ?dup else 0 -1 then until r> drop
    source-id close-file drop throw ;
: INTERPRET-FILE ( i*x fileid xt -- j*x )
    swap ['] interpret-source with-input-source ;
: INCLUDE-FILE ( i*x fileid -- j*x ) ['] interpret interpret-file ;
: INCLUDED  ( i*x c-addr u -- j*x )
    r/o open-file abort" OPEN-FILE failed in INCLUDED" include-file ;
