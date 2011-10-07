\ ------  ------  ------  ------  ------  ------  ------
\ index:
\ IMMEDIATE             6.1.1710 CORE                   41
\ LITERAL               6.1.1780 CORE                   42
\ POSTPONE              6.1.2033 CORE                   43
\ S"                    6.1.2165 CORE                   45
\ [']                   6.1.2510 CORE                   50
\ [CHAR]                6.1.2520 CORE                   50
\ C"                    6.2.0855 CORE EXT               53
\ COMPILE,              6.2.0945 CORE EXT               54
\ [COMPILE]             6.2.2530 CORE EXT               60

\ ------  ------  ------  ------  ------  ------  ------
\ basic compilation - required
: IMMEDIATE
    curdef lastdef OVER <> IF
    \ require the current definition to be complete
    \ XXX mark the definition as IMMEDIATE
;

\ XXX COMPILE, should enforce that there's a current definition
: COMPILE, ( xt -- ) , ;

: [COMPILE]
    BL WORD FIND IF COMPILE, ELSE DROP -13 THROW THEN
; IMMEDIATE

: LITERAL ( x -- )
    \ FIXME - [COMPILE] DO-LITERAL
    , ;

: ['] ( "<spaces>name" -- ) ' POSTPONE LITERAL ; IMMEDIATE
: [CHAR] ( "<spaces>name" -- ) POSTPONE CHAR POSTPONE LITERAL ;
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
