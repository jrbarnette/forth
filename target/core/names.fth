\ '                     6.1.0070 CORE                   25
\ :                     6.1.0450 CORE                   30
\ ;                     6.1.0460 CORE                   30
\ >BODY                 6.1.0550 CORE                   31
\ CONSTANT              6.1.0950 CORE                   35
\ CREATE                6.1.1000 CORE                   36
\ DOES>                 6.1.1250 CORE                   37
\ EXIT                  6.1.1380 CORE                   39
\ FIND                  6.1.1550 CORE                   39
\ IMMEDIATE             6.1.1710 CORE                   41
\ POSTPONE              6.1.2033 CORE                   43
\ RECURSE               6.1.2120 CORE                   44
\ VARIABLE              6.1.2410 CORE                   48
\ :NONAME               6.2.0455 CORE EXT               52
\ COMPILE,              6.2.0945 CORE EXT               54
\ MARKER                6.2.1850 CORE EXT               56
\ TO                    6.2.2295 CORE EXT               59
\ VALUE                 6.2.2405 CORE EXT               60
\ [COMPILE]             6.2.2530 CORE EXT               60


: sfind ( c-addr len -- 0 | xt 1 | xt -1 )
;

: FIND COUNT sfind ;

: COMPILE,
    require-compile
    \ XXX require current definition to be open
    ,
;

: [COMPILE]
    require-compile
    BL WORD FIND IF COMPILE, ELSE DROP -32 THROW THEN
; IMMEDIATE

: POSTPONE
    require-compile
    BL WORD FIND ?DUP IF ( xt -1 | xt 1 )
	0< IF [COMPILE] LITERAL [COMPILE] COMPILE, ELSE COMPILE, THEN
    ELSE ( c-addr )
	DROP -32 THROW
    THEN
; IMMEDIATE

: ' BL WORD FIND 0= IF DROP -32 THROW THEN ;

: IMMEDIATE
    curdef lastdef OVER <> IF
    \ require the current definition to be complete
    \ XXX mark the definition as IMMEDIATE
;

: RECURSE
    require-compile
    \ XXX RECURSE
; IMMEDIATE

: FORGET
    \ XXX FORGET
;

: MARKER
    \ XXX MARKER
;

: : require-interpret X-: startdef ] ;
: ; require-compile POSTPONE EXIT enddef [ ; IMMEDIATE

: DOES> curdef >DATAP HERE ! ; IMMEDIATE
: CREATE X-CREATE startdef enddef DOES> ;
: >BODY [ 2 CELLS ] LITERAL + ;

: CONSTANT CREATE , DOES> @ ;

: VALUE CREATE , DOES> @ ;
: TO
    BL WORD FIND IF
	\ XXX require created by VALUE
	>BODY STATE @ IF POSTPONE LITERAL POSTPONE ! ELSE ! THEN
    ELSE
	DROP -32 THROW
    THEN
; IMMEDIATE

: VARIABLE CREATE [ 1 CELLS ] LITERAL ALLOT ;
