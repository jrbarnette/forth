\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  +LOOP                 6.1.0140 CORE
\  BEGIN                 6.1.0760 CORE
\  DO                    6.1.1240 CORE
\  ELSE                  6.1.1310 CORE
\  IF                    6.1.1700 CORE
\  LEAVE                 6.1.1760 CORE
\  REPEAT                6.1.2140 CORE
\  THEN                  6.1.2270 CORE
\  UNTIL                 6.1.2390 CORE
\  WHILE                 6.1.2430 CORE
\
\  ?DO                   6.2.0620 CORE EXT
\  AGAIN                 6.2.0700 CORE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------

\ FORTH-83 - System Extension Word Set
: <MARK ( -- dest ) here ; compile-only
: <RESOLVE ( dest -- ) here - [ 1 cells ] literal / , ; compile-only
: >MARK ( -- orig ) here [ 1 cells ] literal allot ; compile-only
: >RESOLVE ( orig -- ) here over - [ 1 cells ] literal / swap ! ; compile-only

\ Building blocks - non-standard.
: >BRANCH ( -- orig ) postpone branch >mark ; compile-only
: >?BRANCH ( -- orig ) postpone ?branch >mark ; compile-only
: <BRANCH ( dest -- ) postpone branch <resolve ; compile-only
: <?BRANCH ( dest -- ) postpone ?branch <resolve ; compile-only

\ Basic flow of control words
: BEGIN ( C: -- dest ) <mark ; compile-special
: THEN ( C: orig -- ) >resolve ; compile-special
: IF ( C: -- orig ) >?branch ; compile-special
: UNTIL ( C: dest -- ) <?branch ; compile-special
: ELSE ( C: orig1 -- orig2 ) >branch swap postpone then ; compile-special
: AGAIN <branch ; compile-special

: REPEAT postpone again postpone then ; compile-special
: WHILE ( C: dest -- orig dest ) postpone if swap ; compile-special

\ Flow of control for constructs with internal branches to a common
\ exit point, including DO, ?DO, and CASE.
here 0 ,
: chain-swap ( new-chain -- old-chain )
    [ over ] literal @ swap [ swap ] literal ! ;
: start-chain ( -- saved-chain ) 0 chain-swap ;
: mark>chain ( orig -- ) dup chain-swap swap ! ;
: branch>chain ( -- ) >branch mark>chain ;
: resolve-chain ( saved-chain -- ) 
    chain-swap begin dup while dup @ swap postpone then repeat drop ;

\ Flow of control using DO, ?DO, +LOOP, and LEAVE
\ Compilation of DO-loops
\     DO a +LOOP ->   DO-DO BEGIN a DO-+LOOP UNTIL UNLOOP
\ Compilation of ?DO-loops
\     ?DO a +LOOP ->  DO-DO R@ IF BEGIN a DO-+LOOP UNTIL THEN UNLOOP
\
\ LEAVE compiles as a forward branch to the UNLOOP at the very end.

: DO ( C: -- saved-chain dest )
    postpone do-do start-chain postpone begin ; compile-special
: ?DO ( C: -- saved-chain dest )
    postpone do-do postpone r@
    start-chain postpone if mark>chain postpone begin ; compile-special

: LEAVE branch>chain ; compile-special

: +LOOP ( C: saved-chain dest -- )
    postpone do-+loop postpone until resolve-chain postpone unloop
; compile-special
: LOOP 1 postpone literal postpone +loop ; compile-special
