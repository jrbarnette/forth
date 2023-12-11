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

: id, ( c-addr u -- name )
    here >r counted, r> count 0 do dup c@ toupper over c! char+ loop drop ;

: name, ( c-addr u -- name )
    align here >r current-name , id, align , r> ;

: check-name-length ( u -- u )
    dup 0= if -16 .error then
    dup nf-length u> if -19 .error then ;
: start-name ( hdlr "name" -- name )
    parse-name check-name-length name, ;
: create-name ( hdlr "name" -- )  start-name link-name ;

: : handler: do_colon start-name [compile] ] ;
: ; postpone EXIT ?dup if link-name then [compile] [ ; compile-special

: created? dup @ handler: do_create = invert if -31 .error then ;
: >BODY ( xt -- a-addr ) created? [ 2 cells ] literal + ;

: CONSTANT handler: do_constant create-name , ;

: DOES> r> current-name name>xt created? cell+ ! ; compile-only
: CREATE handler: do_create create-name 0 , DOES> ;

: IMMEDIATE        nf-immediate        current-name name-flags! ;
: COMPILE-ONLY     nf-compile-only     current-name name-flags! ;
: COMPILE-SPECIAL  nf-compile-special  current-name name-flags! ;

: VARIABLE handler: do_variable create-name 0 , ;

: :NONAME align here 0 handler: do_colon , [compile] ] ;
