\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  LITERAL               6.1.1780 CORE
\
\  COMPILE,              6.2.0945 CORE EXT
\  PAD                   6.2.2000 CORE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------

: COMPILE, ( xt -- ) , ; no-interpret

: LITERAL do-literal do-literal compile, , ; compile-special

\     PAD - TBD
