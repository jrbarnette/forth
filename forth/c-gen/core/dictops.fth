\ Copyright 2020, by J. Richard Barnette. All Rights Reserved.

\  dictops.fth - Definitions for standard Forth words for dictionary
\      management.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  UNUSED                6.2.2395 CORE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------

: UNUSED ( -- u ) <C> DICTSPACE_END; here - ;
