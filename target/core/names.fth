\ Copyright 2012, by J. Richard Barnette
\ ------  ------  ------  ------  ------  ------  ------

\ also anonymous definitions
: string, ( c-addr u -- ) chars here swap dup allot move ;
: name, ( mcp c-addr u -- name )
    parse-name get-current @ , dup c, string, align ,
;

: link-name get-current ! ;
: create-name ( "<spaces>name" code-addr -- )
    name, link-name
;

: sfind ( c-addr len -- 0 | xt 1 | xt -1 )
    \ FIXME - sfind
;
\ previous definitions

\ FIND                  6.1.1550 CORE                   39
: FIND ( c-addr -- c-addr 0 | xt 1 | xt -1 ) count sfind ;

\ '                     6.1.0070 CORE                   25
: ' ( "<spaces>name" -- xt ) bl word find 0= if drop -13 throw then ;

\ IMMEDIATE             6.1.1710 CORE                   40
: IMMEDIATE ( -- )
;

\ [']                   6.1.2510 CORE                   48
\ interpretation semantics undefined
\ ( -- xt ) runtime semantics
: ['] ( "<spaces>name" -- ) ' postpone literal ; immediate \ no-interpret

\ handler :-mcp
\ :                     6.1.0450 CORE                   30
: : ( C: "<spaces>name" -- colon-sys ) here mcp-: name, ] ;

\ ;                     6.1.0460 CORE                   30
\ interpretation semantics undefined
: ; ( C: colon-sys -- ) postpone exit postpone [ ;
immediate \ compile-only

\ handler constant-mcp
\ CONSTANT              6.1.0950 CORE                   35
: CONSTANT ( "<spaces>name" x -- )
    here mcp-constant create-name ,
;

\ handler variable-mcp
\ VARIABLE              6.1.2410 CORE                   47
: VARIABLE ( "<spaces>name" -- )
    here mcp-variable create-name [ 1 cells ] literal allot
;

\ handler create-mcp
\ CREATE                6.1.1000 CORE                   36
: CREATE ( "<spaces>name" -- )
    here mcp-create create-name [ here 3 cells + ] literal ,
;

\ DOES>                 6.1.1250 CORE                   37
: DOES>
    postpone exit
    get-current @ cell+ count 1f and chars aligned + cell+
    here !
; immediate

\ >BODY                 6.1.0550 CORE                   31
: >body ( xt -- a-addr ) [ 2 cells ] literal + ;

\ VALUE                 6.2.2405 CORE EXT               58
: VALUE CREATE , DOES> @ ;

\ TO                    6.2.2295 CORE EXT               57
\ XXX TO should require name created by VALUE
: TO
    BL WORD FIND IF
	>BODY STATE @ IF POSTPONE LITERAL POSTPONE ! ELSE ! THEN
    ELSE
	DROP -13 THROW
    THEN
; IMMEDIATE

\ :NONAME               6.2.0455 CORE EXT               51
\ MARKER                6.2.1850 CORE EXT               54
