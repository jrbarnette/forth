\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  EXECUTE               6.1.1370 CORE
\  EXIT                  6.1.1380 CORE
\ 
\  THROW               9.6.1.2275 EXCEPTION
\ ------  ------  ------  ------  ------  ------  ------  ------

prim: EXIT          x_exit         compile-only
prim: EXECUTE       x_execute

prim: DO-LITERAL    do_literal     compile-only
prim: DO-S"         do_s_quote     compile-only
prim: DO-C"         do_c_quote     compile-only

prim: THROW         x_throw

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
