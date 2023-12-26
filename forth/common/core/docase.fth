\ Copyright 2023, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  +LOOP                 6.1.0140 CORE
\  DO                    6.1.1240 CORE
\  LEAVE                 6.1.1760 CORE
\  LOOP                  6.1.1800 CORE
\
\  ?DO                   6.2.0620 CORE EXT
\  CASE                  6.2.0873 CORE EXT
\  ENDCASE               6.2.1342 CORE EXT
\  ENDOF                 6.2.1343 CORE EXT
\  OF                    6.2.1950 CORE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------


: branch>chain ( -- ) >branch mark>chain ;

\ Flow of control using DO, ?DO, +LOOP, and LEAVE
\ Compilation of DO-loops
\     DO a +LOOP ->   DO-DO BEGIN a DO-+LOOP UNTIL UNLOOP
\ Compilation of ?DO-loops
\     ?DO a +LOOP ->  DO-DO R@ IF BEGIN a DO-+LOOP UNTIL THEN UNLOOP
\
\ LEAVE compiles as a forward branch to the UNLOOP at the very end.

: do-prolog postpone do-do start-chain ;
: do-epilog postpone begin ;
: DO ( C: -- saved-chain dest ) do-prolog do-epilog ; compile-special
: ?DO ( C: -- saved-chain dest )
    do-prolog postpone r@ postpone if mark>chain do-epilog ; compile-special

: LEAVE branch>chain ; compile-special

: +LOOP ( C: saved-chain dest -- )
    postpone do-+loop postpone until resolve-chain postpone unloop
; compile-special
: LOOP 1 postpone literal postpone +loop ; compile-special

\ CASE structure/syntax:
\     a CASE b OF c ENDOF ENDCASE
\       -> a b OVER = IF DROP c ELSE DROP THEN
\     a CASE b OF c ENDOF d OF e ENDOF ENDCASE
\       -> a b OVER = IF DROP c ELSE d OVER = IF DROP e ELSE DROP THEN THEN
\ etc.

: CASE ( C: -- saved-chain ) start-chain ; compile-special
: OF ( C: saved-chain -- saved-chain orig )
    postpone over postpone = postpone if postpone drop
; compile-special
: ENDOF ( C: saved-chain orig -- saved-chain )
    branch>chain postpone then
; compile-special
: ENDCASE ( C: saved-chain -- )
    postpone drop resolve-chain
; compile-special
