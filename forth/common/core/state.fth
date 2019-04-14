\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  STATE                 6.1.2250 CORE
\  [                     6.1.2500 CORE
\  ]                     6.1.2540 CORE
\ ------  ------  ------  ------  ------  ------  ------  ------

variable STATE 0 state !

: [ 0 state ! ; immediate
: ] -1 state ! ;
