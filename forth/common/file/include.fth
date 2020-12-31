\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  INCLUDE-FILE       11.6.1.1717 FILE
\  INCLUDED           11.6.1.1718 FILE
\ ------  ------  ------  ------  ------  ------  ------  ------

: INTERPRET-FILE ( i*x fileid -- j*x )
    source<file
    begin refill if ['] interpret catch ?dup else 0 -1 then until
    source-id close-file drop throw ;
: INCLUDE-FILE ( i*x fileid -- j*x )  ['] interpret-file with-nested-source ;
: INCLUDED  ( i*x c-addr u -- j*x )
    r/o open-file abort" OPEN-FILE failed in INCLUDED" include-file ;
