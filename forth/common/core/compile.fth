\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  '                     6.1.0070 CORE
\  (                     6.1.0080 CORE
\  CHAR                  6.1.0895 CORE
\  POSTPONE              6.1.2033 CORE
\  S"                    6.1.2165 CORE
\  [']                   6.1.2510 CORE
\  [CHAR]                6.1.2520 CORE
\
\  .(                    6.2.0200 CORE EXT
\  C"                    6.2.0855 CORE EXT
\  \                     6.2.2535 CORE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------

: ' parse-name lookup dup 0= if -13 throw then name>xt ;
: ( [char] ) parse drop drop ; immediate
: CHAR parse-name 0= if -16 throw then c@ ;
: POSTPONE
    parse-name lookup name>xt+flags immediate?
    0= if postpone literal ['] compile, then compile, ; compile-only
: S" '"' parse postpone do-s" dup , chars, align ; compile-only
: ['] ' postpone literal ; compile-only
: [CHAR] char postpone literal ; compile-only

: .( [char] ) parse type ; immediate
: C" '"' parse postpone do-c" counted, align ; compile-only
: \ source >in ! drop ; immediate
