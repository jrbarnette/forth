\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\  control.fth - Forth definitions for standard words relating to flow
\      of control in compiled defintions.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  +LOOP                 6.1.0140 CORE                   27
\  BEGIN                 6.1.0760 CORE                   34
\  DO                    6.1.1240 CORE                   36
\  ELSE                  6.1.1310 CORE                   37
\  I                     6.1.1680 CORE                   39
\  IF                    6.1.1700 CORE                   40
\  J                     6.1.1730 CORE                   40
\  LEAVE                 6.1.1760 CORE                   41
\  REPEAT                6.1.2140 CORE                   44
\  THEN                  6.1.2270 CORE                   46
\  UNLOOP                6.1.2380 CORE                   47
\  UNTIL                 6.1.2390 CORE                   47
\  WHILE                 6.1.2430 CORE                   47
\
\  ?DO                   6.2.0620 CORE EXT               51
\  AGAIN                 6.2.0700 CORE EXT               51
\ ------  ------  ------  ------  ------  ------  ------  ------

\ Building blocks - non-standard.
: >BRANCH ( -- orig ) postpone branch >mark ;
: <BRANCH ( dest -- ) postpone branch <resolve ;
: >?BRANCH ( -- orig ) postpone ?branch >mark ;
: <?BRANCH ( dest -- ) postpone ?branch <resolve ;

\ Basic flow of control words
: BEGIN ( C: -- dest ) <mark ; compile-only
: THEN ( C: orig -- ) >resolve ; compile-only
: IF ( C: -- orig ) >?branch ; compile-only
: UNTIL ( C: dest -- ) <?branch ; compile-only
: ELSE ( C: orig1 -- orig2 ) >branch swap postpone then ; compile-only
: AGAIN <branch ; compile-only

: REPEAT postpone again postpone then ; compile-only
: WHILE ( C: dest -- orig dest ) postpone if swap ; compile-only

\ Flow of control using DO, ?DO, +LOOP, and LEAVE
variable LEAVERS 0 leavers !

\ ( do-sys ) implemented as ( saved-leavers dest )
: DO-DOPREAMBLE ( -- saved-leavers ) leavers @ postpone do-do ;
: DO-DOBODY ( init-leavers -- dest ) leavers ! postpone begin ;

\ Compilation of DO-loops
\     DO a +LOOP ->   DO-DO a DO-+LOOP UNTIL UNLOOP
\ Compilation of ?DO-loops
\     ?DO a +LOOP ->  DO-DO R@ IF BEGIN a DO-+LOOP UNTIL THEN UNLOOP
\
\ LEAVE compiles as a forward branch to the UNLOOP at the very end.

: DO ( C: -- saved-leavers dest ) do-dopreamble 0 do-dobody ; compile-only
: ?DO ( C: -- saved-leavers dest )
    do-dopreamble postpone r@ postpone if 0 over ! do-dobody ; compile-only

: LEAVE >branch leavers @ over ! leavers ! ; compile-only

: +LOOP ( C: saved-leavers dest -- )
    postpone do-+loop postpone until leavers @ swap leavers !
    begin dup while dup @ swap postpone then repeat drop postpone unloop
; compile-only
: LOOP 1 postpone literal postpone +loop ; compile-only
