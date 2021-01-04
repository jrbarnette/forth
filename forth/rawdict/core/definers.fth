\ Copyright 2020, by J. Richard Barnette. All Rights Reserved.

: : [ handler: do_colon ] literal create-name [compile] ] ;
: ; postpone EXIT link-name [compile] [ ; compile-special

: created? dup @ handler: do_create = invert if .error -31 throw then ;
: >BODY ( xt -- a-addr ) created? [ 2 cells ] literal + ;

: CONSTANT handler: do_constant add-name , ;

\ : DOES> r> current-name name>xt created? cell+ ! ; compile-only
\ : CREATE handler: do_create add-name 0 , DOES> ;

: IMMEDIATE        nf-immediate        current-name name-flags! ;
: COMPILE-ONLY     nf-compile-only     current-name name-flags! ;
: COMPILE-SPECIAL  nf-compile-special  current-name name-flags! ;

: VARIABLE handler: do_variable add-name 0 , ;
