\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  POSTPONE              6.1.2033 CORE                   43
\  S"                    6.1.2165 CORE                   44
\
\  C"                    6.2.0855 CORE EXT               52
\ ------  ------  ------  ------  ------  ------  ------  ------

prim: POSTPONE      x_postpone          compile-only
handler: do_postpone   ,

prim: S"            x_s_quote           compile-only
handler: do_s_quote    ,

prim: C"            x_c_quote           compile-only
handler: do_c_quote    ,
