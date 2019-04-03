\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  CHAR                  6.1.0895 CORE                   35
\  POSTPONE              6.1.2033 CORE                   43
\  S"                    6.1.2165 CORE                   44
\
\  C"                    6.2.0855 CORE EXT               52
\ ------  ------  ------  ------  ------  ------  ------  ------

prim: CHAR          x_char
prim: POSTPONE      x_postpone          compile-only
<C> do_postpone;   ,

prim: S"            x_s_quote           compile-only
<C> do_s_quote;    ,

prim: C"            x_c_quote           compile-only
<C> do_c_quote;    ,
