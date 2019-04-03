\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\  interpret.fth - Outer (interactive) interpreter, and related Forth
\    words.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  ABORT                 6.1.0670 CORE                   32
\  BASE                  6.1.0750 CORE                   34
\  CHAR                  6.1.0895 CORE                   35
\  DECIMAL               6.1.1170 CORE                   36
\  EVALUATE              6.1.1360 CORE                   38
\  POSTPONE              6.1.2033 CORE                   43
\  QUIT                  6.1.2050 CORE                   43
\  S"                    6.1.2165 CORE                   44
\  STATE                 6.1.2250 CORE                   45
\  [                     6.1.2500 CORE                   48
\  ]                     6.1.2540 CORE                   49
\
\  C"                    6.2.0855 CORE EXT               52
\  HEX                   6.2.1660 CORE EXT               54
\ ------  ------  ------  ------  ------  ------  ------  ------

prim: ABORT         x_abort

<C> &DICT.base;
constant BASE

prim: CHAR          x_char
prim: DECIMAL       x_decimal
prim: EVALUATE      x_evaluate
prim: POSTPONE      x_postpone          compile-only
<C> do_postpone;   ,

prim: QUIT          x_quit

<C> &DICT.state;
constant STATE

prim: S"            x_s_quote           compile-only
<C> do_s_quote;    ,

prim: [             x_left_bracket      compile-only
prim: ]             x_right_bracket

prim: C"            x_c_quote           compile-only
<C> do_c_quote;    ,

prim: HEX           x_hex