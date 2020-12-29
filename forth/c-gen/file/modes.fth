\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  BIN                11.6.1.0765 FILE
\  R/O                11.6.1.2054 FILE
\  R/W                11.6.1.2056 FILE
\  W/O                11.6.1.2425 FILE
\ ------  ------  ------  ------  ------  ------  ------  ------

<C> FILE_MODE_R_O;  constant R/O
<C> FILE_MODE_R_W;  constant R/W
<C> FILE_MODE_W_O;  constant W/O

: BIN ( fam1 -- fam2 )  <C> FILE_MODE_BIN; or ;
