\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\  Contains all semantics for the following standard definitions:
\ ------  ------  ------  ------  ------  ------  ------  ------
\  EXECUTE               6.1.1370 CORE
\  EXIT                  6.1.1380 CORE
\ ------  ------  ------  ------  ------  ------  ------  ------

\  Contains runtime or other partial semantics for the following:
\ ------  ------  ------  ------  ------  ------  ------  ------
\  LITERAL               6.1.1780 CORE
\  S"                    6.1.2165 CORE
\
\  C"                    6.2.0855 CORE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------

prim: EXECUTE       x_execute
prim: EXIT          x_exit         compile-only

prim: DO-LITERAL    do_literal     compile-only
prim: DO-S"         do_s_quote     compile-only
prim: DO-C"         do_c_quote     compile-only

prim: CLEAR         x_clear
prim: RCLEAR        x_rclear

\ prim: SP0
\ prim: SP@
\ prim: SP!
\ : CLEAR sp0 sp! ;
\ prim: RP0
\ prim: RP@
\ prim: RP!
\ : RCLEAR rp0 rp! ;
