\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\  vmprim.fth - Forth primitives for inner VM operation.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  EXECUTE               6.1.1370 CORE                   38
\  EXIT                  6.1.1380 CORE                   38
\  LITERAL               6.1.1780 CORE                   41
\ 
\  THROW               9.6.1.2275 EXCEPTION              73
\ ------  ------  ------  ------  ------  ------  ------  ------

prim: EXIT          x_exit         no-interpret
prim: EXECUTE       x_execute

: LITERAL [ <C> DO_LITERAL_XT; dup , , ] , , ; compile-only

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
