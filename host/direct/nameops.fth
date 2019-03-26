\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\  nameops.fth - Forth code to implement operations relating to
\      creating definitions.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  DOES>                 6.1.1250 CORE                   37
\ ------  ------  ------  ------  ------  ------  ------  ------

prim: DOES!             do_does
: DO-DOES> r> does! ;
: DOES> postpone do-does> ; compile-only
