\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\  control.fth - Building blocks for standard Forth words relating
\      to flow of control in compiled defintions.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  This file defines the following standard words:
\  I                     6.1.1680 CORE                   39
\  J                     6.1.1730 CORE                   40
\  UNLOOP                6.1.2380 CORE                   47
\
\  Definitions here support the following standard words:
\  +LOOP                 6.1.0140 CORE                   27
\  BEGIN                 6.1.0760 CORE                   34
\  DO                    6.1.1240 CORE                   36
\  ELSE                  6.1.1310 CORE                   37
\  IF                    6.1.1700 CORE                   40
\  LEAVE                 6.1.1760 CORE                   41
\  REPEAT                6.1.2140 CORE                   44
\  THEN                  6.1.2270 CORE                   46
\  UNTIL                 6.1.2390 CORE                   47
\  WHILE                 6.1.2430 CORE                   47
\
\  ?DO                   6.2.0620 CORE EXT               51
\  AGAIN                 6.2.0700 CORE EXT               51
\ ------  ------  ------  ------  ------  ------  ------  ------

\ FORTH-83 - System Extension Word Set
prim: BRANCH      do_skip
prim: ?BRANCH     do_fskip
: <MARK ( -- dest ) here ; compile-only
: <RESOLVE ( dest -- ) here - [ 1 cells ] literal / , ; compile-only
: >MARK ( -- orig ) here [ 1 cells ] literal allot ; compile-only
: >RESOLVE ( orig -- ) here over - [ 1 cells ] literal / swap ! ; compile-only

\ Internal primitive building blocks for DO loops.
prim: DO-DO       do_do           compile-only
prim: DO-+LOOP    do_plus_loop    compile-only

\ Standard primitives for DO loops.
prim: UNLOOP      x_unloop        compile-only
prim: I           x_i             compile-only
prim: J           x_j             compile-only
