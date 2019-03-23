\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\  LITERAL               6.1.1780 CORE                   41
\  RECURSE               6.1.2120 CORE                   43
\  S"                    6.1.2165 CORE                   44
\  [']                   6.1.2510 CORE                   48
\  [CHAR]                6.1.2520 CORE                   49
\ ------  ------  ------  ------  ------  ------  ------  ------

: LITERAL ( x -- ) postpone do-literal , ; immediate \ no-interpret
: [CHAR] ( "<spaces>name" -- ) char postpone literal ;
immediate \ no-interpret
: ['] ( "<spaces>name" -- ) ' postpone literal ; immediate \ no-interpret

: S" ( "ccc<quote>" -- )
    \ FIXME - [compile] do-s"
    [char] " parse dup , here swap move
;


\ COMPILE, and [COMPILE] deliberately out of order
\ XXX COMPILE, should enforce that there's a current definition
: COMPILE, ( xt -- ) , ;
: [COMPILE] ( "<spaces>name" -- ) ' compile, ; immediate \ no-interpret

: POSTPONE ( "<spaces>name" -- )
    parse-name lookup 0= if -13 throw then
    nf-immediate and 0= if [compile] literal ['] compile, then compile,
; immediate \ no-interpret

\ ------  ------  ------  ------  ------  ------  ------  ------
\  C"                    6.2.0855 CORE EXT               52
\  COMPILE,              6.2.0945 CORE EXT               52
\  [COMPILE]             6.2.2530 CORE EXT               58
\ ------  ------  ------  ------  ------  ------  ------  ------

: C" ( "ccc<quote>" -- )
    \ FIXME - [compile] do-c"
    [char] " parse dup c, here swap move
;
