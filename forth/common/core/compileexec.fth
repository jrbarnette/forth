\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  LITERAL               6.1.1780 CORE                   41
\
\  COMPILE,              6.2.0945 CORE EXT
\  PAD                   6.2.2000 CORE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------

: LITERAL do-literal do-literal , , ; compile-only

: COMPILE, ( xt -- ) , ; no-interpret

\     PAD - TBD
