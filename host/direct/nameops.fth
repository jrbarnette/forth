\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\  nameops.fth - Forth code to implement operations relating to
\      creating definitions.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  >BODY                 6.1.0550 CORE                   31
\  CREATE                6.1.1000 CORE                   36
\  DOES>                 6.1.1250 CORE                   37
\ ------  ------  ------  ------  ------  ------  ------  ------

prim: >BODY     x_to_body
prim: DO-DOES>  do_does

: DOES> r> do-does> ; no-interpret

here <C> x_create; ,
: CREATE [ swap , ] [ 1 cells ] literal allot DOES> ;
