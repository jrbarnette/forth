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
: <MARK ( -- dest ) here ;
: <RESOLVE ( dest -- ) here - [ 1 cells ] literal / , ;
: >MARK ( -- orig ) here [ 1 cells ] literal allot ;
: >RESOLVE ( orig -- ) here over - [ 1 cells ] literal / swap ! ;

\ prim: BRANCH    do_skip
here <C> do_skip; ,
: >BRANCH ( -- orig ) [ over ] literal , >mark ;
: <BRANCH ( dest -- ) [ swap ] literal , <resolve ;

\ prim: ?BRANCH   do_fskip
here <C> do_fskip; ,
: >?BRANCH ( -- orig ) [ over ] literal , >mark ;
: <?BRANCH ( dest -- ) [ swap ] literal , <resolve ;

: BEGIN ( C: -- dest ) <mark ; compile-only
: THEN ( C: orig -- ) >resolve ; compile-only
: IF ( C: -- orig ) >?branch ; compile-only
: UNTIL ( C: dest -- ) <?branch ; compile-only
\ : AGAIN <branch ; compile-only
\ : REPEAT [compile] again [compile] then ; compile-only
: REPEAT ( C: orig dest -- ) <branch >resolve ; compile-only
: ELSE ( C: orig1 -- orig2 ) >branch swap >resolve ; compile-only
: WHILE ( C: dest -- orig dest ) >?branch swap ; compile-only

\ prim: +LOOP     x_plus_loop     compile-only
\ [compile] UNLOOP

\ prim: DO        x_do            compile-only
\ prim: LEAVE     c_leave         compile-only

variable LEAVERS 0 leavers !

here <C> do_do; ,
: DO ( C: -- do-sys )
    [ swap ] literal , leavers @ 0 leavers ! <mark ; compile-only
: LEAVE >branch leavers @ over ! leavers ! ; compile-only

prim: UNLOOP    x_unloop        no-interpret
here 1 cells -
prim: I         x_i             no-interpret
prim: J         x_j             no-interpret

here <C> do_plus_loop; ,
: +LOOP ( C: do-sys -- )
    [ swap ] literal , <?branch leavers @ swap leavers !
    begin dup while dup >resolve @ repeat drop [ swap ] literal ,
; compile-only
