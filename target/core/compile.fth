\ Copyright 2012, by J. Richard Barnette
\ ------  ------  ------  ------  ------  ------  ------
\ POSTPONE              6.1.2033 CORE                   43
\ RECURSE               6.1.2120 CORE                   43
\ S"                    6.1.2165 CORE                   44
\ STATE                 6.1.2250 CORE                   45
\ [                     6.1.2500 CORE                   48
\ ]                     6.1.2540 CORE                   49
\ C"                    6.2.0855 CORE EXT               52
\ [COMPILE]             6.2.2530 CORE EXT               58

\ ------  ------  ------  ------  ------  ------  ------
\ basic compilation - required

\ CHAR                  6.1.0895 CORE                   35
: CHAR ( "<spaces>name" --- char ) parse-name drop c@ ;

\ COMPILE,              6.2.0945 CORE EXT               52
\ XXX COMPILE, should enforce that there's a current definition
: COMPILE, ( xt -- ) , ;

: [COMPILE]
    BL WORD FIND IF COMPILE, ELSE DROP -13 THROW THEN
; IMMEDIATE

\ anonymous defintions
\ create-prim do-literal
\ target definitions

\ LITERAL               6.1.1780 CORE                   41
\ interpretation semantics undefined
\ ( -- x ) runtime semantics
: LITERAL ( x -- ) postpone do-literal , ; immediate \ no-interpret

\ [CHAR]                6.1.2520 CORE                   49
\ interpretation semantics undefined
\ ( -- char ) runtime semantics
: [CHAR] ( "<spaces>name" -- ) postpone char postpone literal ;
immediate \ no-interpret

: S" ( "ccc<quote>" -- )
    \ FIXME - [COMPILE] DO-S"
    [CHAR] " PARSE DUP , HERE SWAP MOVE ;

: C" ( "ccc<quote>" -- )
    \ FIXME - [COMPILE] DO-C"
    [CHAR] " PARSE DUP C, HERE SWAP MOVE ;

: POSTPONE ( "<spaces>name" -- )
    BL WORD FIND ?DUP IF ( xt -1 | xt 1 )
	0< IF [COMPILE] LITERAL [COMPILE] COMPILE, ELSE COMPILE, THEN
    ELSE ( c-addr )
	DROP -13 THROW
    THEN
; IMMEDIATE
