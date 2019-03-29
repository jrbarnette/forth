\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\  vmprim.fth - Forth primitives for inner VM operation.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  EXECUTE               6.1.1370 CORE                   38
\  EXIT                  6.1.1380 CORE                   38
\ 
\  THROW               9.6.1.2275 EXCEPTION              73
\ ------  ------  ------  ------  ------  ------  ------  ------

prim: EXIT          x_exit         no-interpret
prim: EXECUTE       x_execute

prim: THROW         x_throw
