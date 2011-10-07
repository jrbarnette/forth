\ ------  ------  ------  ------  ------  ------  ------
\ index:
\ '                     6.1.0070 CORE                   25
\ :                     6.1.0450 CORE                   30
\ ;                     6.1.0460 CORE                   30
\ >BODY                 6.1.0550 CORE                   31
\ CONSTANT              6.1.0950 CORE                   35
\ CREATE                6.1.1000 CORE                   36
\ DOES>                 6.1.1250 CORE                   37
\ EXIT                  6.1.1380 CORE                   39
\ FIND                  6.1.1550 CORE                   39
\ RECURSE               6.1.2120 CORE                   44
\ VARIABLE              6.1.2410 CORE                   48
\ :NONAME               6.2.0455 CORE EXT               52
\ MARKER                6.2.1850 CORE EXT               56
\ TO                    6.2.2295 CORE EXT               59
\ VALUE                 6.2.2405 CORE EXT               60

\ ------  ------  ------  ------  ------  ------  ------

\ EXIT ( -- ) ( R: nest-sys -- )

: sfind ( c-addr len -- 0 | xt 1 | xt -1 )
    \ FIXME - sfind
;

: FIND COUNT sfind ;

: ' BL WORD FIND 0= IF DROP -13 THROW THEN ;

: RECURSE
    \ XXX RECURSE
; IMMEDIATE

: MARKER
    \ XXX MARKER
;

: : ( C: "name<space>" -- colon-sys )
    BL PARSE
    \ FIXME - :
    \ enter name in dictionary; mark "incomplete"
    \ PRIMITIVE DO-: ,
    ] ;
: ; ( C: colon-sys -- ) POSTPONE EXIT enddef [ ; IMMEDIATE

: DOES> ( C: colon-sys1 -- colon-sys2 )
    \ FIXME - DOES>
    \ XXX get pointer to DOES> slot of last CREATE
    HERE ! ; IMMEDIATE
: CREATE ( "name<space>" -- )
    \ FIXME - CREATE
    \ PRIMITIVE DO-CREATE ,
    [ HERE 2 CELLS + ] LITERAL , ;

\ XXX >BODY should require xt created by CREATE
: >BODY ( xt -- a-addr ) [ 2 CELLS ] LITERAL + ;

: CONSTANT CREATE , DOES> @ ;

: VALUE CREATE , DOES> @ ;
\ XXX TO should require name created by VALUE
: TO
    BL WORD FIND IF
	>BODY STATE @ IF POSTPONE LITERAL POSTPONE ! ELSE ! THEN
    ELSE
	DROP -13 THROW
    THEN
; IMMEDIATE

: VARIABLE CREATE [ 1 CELLS ] LITERAL ALLOT ;
