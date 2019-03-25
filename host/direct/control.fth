\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\  control.fth - Standard Forth words relating to flow of control in
\    compiled defintions.

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
\ ------  ------  ------  ------  ------  ------  ------  ------

\ FORTH-83 - System Extension Word Set
prim: BRANCH    do_skip
prim: ?BRANCH   do_fskip

: <MARK ( -- dest ) here ;
: <RESOLVE ( dest -- ) here - [ 1 cells ] literal / , ;
: >MARK ( -- orig ) here [ 1 cells ] literal allot ;
: >RESOLVE ( orig -- ) here over - [ 1 cells ] literal / swap ! ;

\ These are non-standard, but useful.
: >BRANCH ( -- orig ) postpone branch >mark ;
: <BRANCH ( dest -- ) postpone branch <resolve ;
: >?BRANCH ( -- orig ) postpone ?branch >mark ;
: <?BRANCH ( dest -- ) postpone ?branch <resolve ;

: BEGIN ( C: -- dest ) <mark ; compile-only
: THEN ( C: orig -- ) >resolve ; compile-only
: IF ( C: -- orig ) >?branch ; compile-only
: UNTIL ( C: dest -- ) <?branch ; compile-only
\ : AGAIN <branch ; compile-only
\ : REPEAT [compile] again [compile] then ; compile-only
: REPEAT ( C: orig dest -- ) <branch postpone then ; compile-only
: ELSE ( C: orig1 -- orig2 ) >branch swap postpone then ; compile-only
: WHILE ( C: dest -- orig dest ) postpone if swap ; compile-only

variable LEAVERS 0 leavers !

here <C> do_do; ,
: DO ( C: -- do-sys )
    [ swap ] literal , leavers @ 0 leavers ! postpone begin ; compile-only
: LEAVE >branch leavers @ over ! leavers ! ; compile-only

prim: UNLOOP    x_unloop        no-interpret
here 1 cells -
prim: I         x_i             no-interpret
prim: J         x_j             no-interpret

here <C> do_plus_loop; ,
: +LOOP ( C: do-sys -- )
    [ swap ] literal , postpone until leavers @ swap leavers !
    begin dup while dup postpone then @ repeat drop [ swap ] literal ,
; compile-only
