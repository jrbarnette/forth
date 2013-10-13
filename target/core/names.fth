\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\ '                     6.1.0070 CORE                   25
\ :                     6.1.0450 CORE                   30
\ ;                     6.1.0460 CORE                   30
\ >BODY                 6.1.0550 CORE                   31
\ CONSTANT              6.1.0950 CORE                   35
\ CREATE                6.1.1000 CORE                   36
\ DOES>                 6.1.1250 CORE                   37
\ FIND                  6.1.1550 CORE                   39
\ IMMEDIATE             6.1.1710 CORE                   40
\ VARIABLE              6.1.2410 CORE                   47
\ ------  ------  ------  ------  ------  ------  ------  ------

\ also anonymous definitions
\ parse-name ( -- c-addr u )
: string, ( c-addr u -- ) chars here swap dup allot move ;
: name, ( mcp -- name )
    here swap current-name @ , parse-name dup c, string, align ,
;
: link-name ( name -- ) current-name ! ;
: create-name ( "<spaces>name" code-addr -- ) name, link-name ;
: sfind ( c-addr len -- 0 | xt 1 | xt -1 ) FIXME ;
\ previous definitions

\ handler :-mcp
\ handler constant-mcp
\ handler variable-mcp
\ handler create-mcp

: FIND ( c-addr -- c-addr 0 | xt 1 | xt -1 ) count sfind ;
: ' ( "<spaces>name" -- xt ) bl word find 0= if drop -13 throw then ;
: IMMEDIATE ( -- ) FIXME ;
: : ( C: "<spaces>name" -- colon-sys ) here mcp-: name, ] ;
: ; ( C: colon-sys -- ) postpone exit postpone [ ;
immediate \ compile-only

: CONSTANT ( "<spaces>name" x -- ) mcp-constant create-name , ;
: VARIABLE ( "<spaces>name" -- )
    mcp-variable create-name [ 1 cells ] literal allot ;
: CREATE ( "<spaces>name" -- )
    mcp-create create-name [ here 3 cells + ] literal , ;
: DOES>
    postpone exit
    current-name @ cell+ count 1f and chars aligned + cell+
    here !
; immediate
: >BODY ( xt -- a-addr ) [ 2 cells ] literal + ;

\ ------  ------  ------  ------  ------  ------  ------  ------
\ :NONAME               6.2.0455 CORE EXT               51
\ MARKER                6.2.1850 CORE EXT               54
\ TO                    6.2.2295 CORE EXT               57
\ VALUE                 6.2.2405 CORE EXT               58
\ ------  ------  ------  ------  ------  ------  ------  ------

: VALUE create , does> @ ;

\ XXX TO should require name created by VALUE
: TO
    bl word find if
	>body state @ if postpone literal postpone ! else ! then
    else
	drop -13 throw
    then
; immediate
