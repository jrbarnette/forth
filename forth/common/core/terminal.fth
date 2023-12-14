\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  BL                    6.1.0770 CORE
\  ACCEPT                6.1.0695 CORE                   33
\  CR                    6.1.0990 CORE                   36
\  SPACE                 6.1.2220 CORE                   45
\  SPACES                6.1.2230 CORE                   45
\  TYPE                  6.1.2310 CORE                   46
\ ------  ------  ------  ------  ------  ------  ------  ------

32 constant BL

80 ( sz-prompt )
here over 1+ chars allot constant PROMPT 0 prompt c!
: PROMPT! ( c-addr u -- )
    prompt swap [ swap ] literal min chars 2dup + >r move 0 r> c! ;

: REFILL-TERMINAL ( c-addr u -- len flag )
    prompt prompt-accept 0 prompt c! ;
: ACCEPT ( c-addr +n1 -- +n2 ) refill-terminal drop ;

: TYPE ( c-addr u -- ) 0 ?do dup i chars + c@ emit loop drop ;

: CR ( -- ) 10 emit ;
: SPACE ( -- ) bl emit ;
: SPACES ( n -- ) dup 0> if 0 do space loop else drop then ;
