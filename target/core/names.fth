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
\ RECURSE               6.1.2120 CORE                   43
\ VARIABLE              6.1.2410 CORE                   47
\ ------  ------  ------  ------  ------  ------  ------  ------

\ anonymous lookup ( c-addr u -- 0 | xt flags -1 )
\ anonymous definitions
: string, ( c-addr u -- ) dup c, here swap chars dup allot move ;
: name, ( mcp -- name )
    align here swap get-current @ , parse-name dup c, string, align ,
;
: link-name ( name -- ) get-current ! ;
: create-name ( "<spaces>name" code-addr -- ) name, link-name ;
80 constant nf-immediate
40 constant nf-no-interpret
: no-interpret get-current @ cell+ dup c@ nf-no-interpret or swap c! ;

\ handler :-mcp
\ handler constant-mcp
\ handler variable-mcp
\ handler create-mcp

\ forth definitions
: FIND ( c-addr -- c-addr 0 | xt 1 | xt -1 )
    dup count lookup if
	rot drop nf-immediate and 0= 1 or
    else
	0
    then
;
: ' ( "<spaces>name" -- xt ) bl word find 0= if drop -13 throw then ;
: IMMEDIATE ( -- ) get-current @ cell+ dup c@ nf-immediate or swap c! ;
: : ( C: "<spaces>name" -- colon-sys ) mcp-: name, ] ;
: ; ( C: colon-sys -- ) link-name postpone exit postpone [ ;
immediate \ compile-only

\ non-standard definition
base @ hex
: >XT ( colon-sys -- xt ) cell+ dup c@ 1f and 1+ chars + aligned ;
base !

: RECURSE ( colon-sys -- colon-sys ) dup >xt compile, ; immediate

: CONSTANT ( "<spaces>name" x -- ) mcp-constant create-name , ;
: VARIABLE ( "<spaces>name" -- ) mcp-variable create-name 0 , ;
: CREATE ( "<spaces>name" -- )
    mcp-create create-name [ here 3 cells + ] literal , ;
: DOES> ( C: colon-sys2 -- colon-sys2 )
    postpone exit here over >xt cell+ !
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
