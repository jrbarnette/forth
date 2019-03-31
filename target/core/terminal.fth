\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  .                     6.1.0180 CORE                   27
\  ."                    6.1.0190 CORE                   28
\  CR                    6.1.0990 CORE                   36
\  SPACE                 6.1.2220 CORE                   45
\  SPACES                6.1.2230 CORE                   45
\  TYPE                  6.1.2310 CORE                   46
\  U.                    6.1.2320 CORE                   46
\
\  .R                    6.2.0210 CORE EXT               49
\  U.R                   6.2.2330 CORE EXT               57
\ ------  ------  ------  ------  ------  ------  ------  ------

: TYPE ( c-addr u -- )
    chars over + swap begin 2dup > while dup c@ emit char+ repeat 2drop
;

: . <# bl hold dup abs 0 #s rot sign #> type ;
: ." postpone s" postpone type ; compile-only
: CR ( -- ) 10 emit ;
: SPACE ( -- ) bl emit ;
: SPACES ( n -- ) begin dup 0> while space 1- repeat drop ;
: U. <# bl hold 0 #s #> type ;

: .R ( n1 n2 -- ) >r <# dup abs 0 #s rot sign #> r> over - spaces type ;
: U.R ( u n -- ) >r <# 0 #s #> r> over - spaces type ;
