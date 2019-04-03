\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  ABORT                 6.1.0670 CORE                   32
\  EVALUATE              6.1.1360 CORE                   38
\  QUIT                  6.1.2050 CORE                   43
\  STATE                 6.1.2250 CORE                   45
\  [                     6.1.2500 CORE                   48
\  ]                     6.1.2540 CORE                   49
\ ------  ------  ------  ------  ------  ------  ------  ------

prim: ABORT         x_abort

prim: EVALUATE      x_evaluate

prim: QUIT          x_quit

<C> &DICT.state;
constant STATE

prim: [             x_left_bracket      compile-only
prim: ]             x_right_bracket
