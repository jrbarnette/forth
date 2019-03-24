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

\ <MARK
: BEGIN ( C: -- dest ) here ; compile-only

\ >RESOLVE
: THEN ( C: orig -- ) here over - [ 1 cells ] literal / swap ! ; compile-only

\ >MARK ( C: -- orig )
here <C> do_colon; , ] here [ 1 cells ] literal allot exit [

\ ?BRANCH
here <C> do_fskip; ,

( C: >mark ?branch )
\ : IF postpone ?branch >mark ; compile-only
: IF ( C: -- orig ) [ over ] literal , [ rot swap over , ] ; compile-only

( C: ?branch >mark )
\ <RESOLVE ( C: dest -- )
here <C> do_colon; , ] here - [ 1 cells ] literal / , exit [

rot ( C: >mark <resolve ?branch )
\ : UNTIL postpone ?branch <resolve ; compile-only
: UNTIL ( C: dest -- ) [ swap ] literal , [ over , ] ; compile-only

( C: >mark <resolve )
\ BRANCH
here <C> do_skip; ,

( C: >mark <resolve branch )
\ : AGAIN postpone branch <resolve ; compile-only
\ : REPEAT postpone again postpone then ; compile-only
: REPEAT ( C: orig dest -- )
    [ over ] literal , [ rot , ] [compile] then ; compile-only

( C: >mark branch )
\ : ELSE postpone branch >mark swap postpone then ; compile-only
: ELSE ( C: orig1 -- orig2 )
    [ swap ] literal , [ swap , ] swap [compile] then ; compile-only
( C: )

: WHILE ( C: dest -- orig dest ) [compile] if swap ; compile-only

prim: UNLOOP    x_unloop        no-interpret
prim: +LOOP     x_plus_loop     compile-only
[compile] UNLOOP

prim: DO        x_do            compile-only
prim: I         x_i             no-interpret
prim: J         x_j             no-interpret
prim: LEAVE     c_leave         compile-only
