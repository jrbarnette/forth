\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\  nameops.fth -

\ ------  ------  ------  ------  ------  ------  ------  ------
\  DOES>                 6.1.1250 CORE                   37
\ ------  ------  ------  ------  ------  ------  ------  ------

prim: DO-DOES>          do_does
: DOES>
    align here cell+ >r
    0 postpone literal postpone do-does> postpone exit
    here r> ! ; compile-only
