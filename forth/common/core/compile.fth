\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  '                     6.1.0070 CORE
\  CHAR                  6.1.0895 CORE
\  POSTPONE              6.1.2033 CORE
\  S"                    6.1.2165 CORE
\  [']                   6.1.2510 CORE
\  [CHAR]                6.1.2520 CORE
\
\  C"                    6.2.0855 CORE EXT
\  [COMPILE]             6.2.2530 CORE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------

: parse-valid-name ( "name" -- nt )
    parse-name lookup dup 0= if -13 .error then ;

: ' ( "name" -- xt )  parse-valid-name name>xt ;
: CHAR ( "name" -- char )  parse-name 0= if -16 .error then c@ ;

: [COMPILE] ' compile, ; compile-special

: ['] ' [compile] literal ; compile-special
: [CHAR] char [compile] literal ; compile-special

: POSTPONE
    parse-valid-name name>xt+flags immediate? 0=
    if [compile] literal ['] compile, then compile, ; compile-special

: S" [char] " parse postpone do-s" dup , chars, align ; compile-special
: C" [char] " parse postpone do-c" counted, align ; compile-special
