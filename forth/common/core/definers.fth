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

: check-name-length ( u -- u )
    dup 0= if -16 throw then
    dup nf-length u> if -19 throw then ;
: start-name ( hdlr "name" -- name )
    parse-name check-name-length name, ;
: create-name ( hdlr "name" -- )  start-name link-name ;

: : hdlr-colon start-name [compile] ] ;
: ; postpone EXIT ?dup if link-name then [compile] [ ; compile-special

: created? dup @ hdlr-create <> if -31 throw then ;
: >BODY ( xt -- a-addr ) created? [ 2 cells ] literal + ;

: CONSTANT hdlr-constant create-name , ;

: DOES> r> current-name name>xt created? cell+ ! ; compile-only
: CREATE hdlr-create create-name 0 , DOES> ;

: IMMEDIATE        nf-immediate        current-name name-flags! ;
: COMPILE-ONLY     nf-compile-only     current-name name-flags! ;
: COMPILE-SPECIAL  nf-compile-special  current-name name-flags! ;

: VARIABLE hdlr-variable create-name 0 , ;

: :NONAME align here 0 hdlr-colon , [compile] ] ;
