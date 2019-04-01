\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\  LITERAL               6.1.1780 CORE                   41
\  POSTPONE              6.1.2033 CORE                   43
\  S"                    6.1.2165 CORE                   44
\  [']                   6.1.2510 CORE                   48
\  [CHAR]                6.1.2520 CORE                   49
\
\  C"                    6.2.0855 CORE EXT               52
\  COMPILE,              6.2.0945 CORE EXT               52
\  [COMPILE]             6.2.2530 CORE EXT               58
\ ------  ------  ------  ------  ------  ------  ------  ------

: CHARS, ( c-addr u -- ) here swap chars dup allot move ;
: STR, ( c-addr u -- ) align dup , chars, ;
: CSTR, ( c-addr u -- ) dup c, chars, ;

: COMPILE, , ; no-interpret
: LITERAL ( x -- ) postpone do-literal , ; compile-only

: POSTPONE
    parse-name lookup name>xt+flags nf-immediate and 0= if
        postpone literal ['] compile,
    then compile,
; compile-only

: S" ( "ccc<quote>" -- ) postpone do-s" [char] " parse str, ;
: [CHAR] ( "<spaces>name" -- ) char postpone literal ; compile-only
: ['] ( "<spaces>name" -- ) ' postpone literal ; compile-only


: [COMPILE] ( "<spaces>name" -- ) ' compile, ; compile-only

: C" ( "ccc<quote>" -- ) postpone do-c" [char] " parse cstr, ;
