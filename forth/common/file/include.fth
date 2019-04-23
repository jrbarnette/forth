\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  INCLUDE-FILE       11.6.1.1717 FILE
\  INCLUDED           11.6.1.1718 FILE
\ ------  ------  ------  ------  ------  ------  ------  ------

: INCLUDE-FILE  ( i*x fileid –– j*x )
    nest-source source<file
    begin refill while interpret repeat
    source-id close-file drop unnest-source ;
: INCLUDED  ( i*x c-addr u –– j*x )
    r/o open-file abort" OPEN-FILE failed in INCLUDED" include-file ;
