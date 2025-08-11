\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  BEGIN                 6.1.0760 CORE
\  ELSE                  6.1.1310 CORE
\  IF                    6.1.1700 CORE
\  REPEAT                6.1.2140 CORE
\  THEN                  6.1.2270 CORE
\  UNTIL                 6.1.2390 CORE
\  WHILE                 6.1.2430 CORE
\
\  AGAIN                 6.2.0700 CORE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------

\ Basic flow of control words
: BEGIN ( C: -- dest ) <mark ; compile-special
: THEN ( C: orig -- ) >resolve ; compile-special
: IF ( C: -- orig ) >?branch ; compile-special
: UNTIL ( C: dest -- ) <?branch ; compile-special
: ELSE ( C: orig1 -- orig2 ) >branch swap postpone then ; compile-special
: AGAIN <branch ; compile-special

: REPEAT postpone again postpone then ; compile-special
: WHILE ( C: dest -- orig dest ) postpone if swap ; compile-special
