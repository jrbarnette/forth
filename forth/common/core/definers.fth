\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  :                     6.1.0450 CORE
\  ;                     6.1.0460 CORE
\  >BODY                 6.1.0550 CORE
\  CONSTANT              6.1.0950 CORE
\  CREATE                6.1.1000 CORE
\  DOES>                 6.1.1250 CORE
\  IMMEDIATE             6.1.1710 CORE
\  VARIABLE              6.1.2410 CORE
\  :NONAME               6.2.0455 CORE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------

: current-name ( -- name ) get-current @ ;
: link-name ( name -- ) get-current ! ;

: ID, ( c-addr u -- )
    dup 0= if .error -16 throw then here >r counted, r>
    count 0 do dup c@ toupper over c! char+ loop drop ;
: NAME, ( hdlr c-addr u -- name )
    align here >r current-name , id, align , r> ;

: create-name ( hdlr "name" -- name )
    parse-name dup 0= if .error -16 throw then name, ;
: add-name ( hdlr "name" -- ) create-name link-name ;

: : handler: do_colon create-name [compile] ] ;
: ; postpone EXIT ?dup if link-name then [compile] [ ; compile-special

: created? dup @ handler: do_create = invert if .error -31 throw then ;
: >BODY ( xt -- a-addr ) created? [ 2 cells ] literal + ;

: CONSTANT handler: do_constant add-name , ;

: DOES> r> current-name name>xt created? cell+ ! ; compile-only
: CREATE handler: do_create add-name 0 , DOES> ;

: IMMEDIATE        nf-immediate        current-name name-flags! ;
: COMPILE-ONLY     nf-compile-only     current-name name-flags! ;
: COMPILE-SPECIAL  nf-compile-special  current-name name-flags! ;

: VARIABLE handler: do_variable add-name 0 , ;

: :NONAME here 0 handler: do_colon , [compile] ] ;
