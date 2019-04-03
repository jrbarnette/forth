\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  (                     6.1.0080 CORE
\  [']                   6.1.2510 CORE
\  [CHAR]                6.1.2520 CORE
\
\  .(                    6.2.0200 CORE EXT
\  \                     6.2.2535 CORE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------

: ( [char] ) parse drop drop ; immediate
: ['] ' postpone literal ; compile-only
: [CHAR] char postpone literal ; compile-only

: .( [char] ) parse type ; immediate
: \ source >in ! drop ; immediate
