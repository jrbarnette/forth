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
\ ------  ------  ------  ------  ------  ------  ------  ------

prim: startname         x_startname
prim: linkname          x_linkname
: addname startname linkname ;

: : <C> do_colon; startname [compile] ] ;
: ; postpone EXIT linkname [compile] [ ; compile-only

: created? dup @ <C> do_create; = invert if -31 throw then ;
: >BODY ( xt -- a-addr ) created? [ 2 cells ] literal + ;

: CONSTANT <C> do_constant; addname , ;

: current-name ( -- name ) get-current @ ;
: name>xt ( name -- xt ) cell+ count 31 and chars + aligned ;
: DOES> r> current-name name>xt created? cell+ ! ; no-interpret
: CREATE <C> do_create; addname 0 , DOES> ;

: flags! current-name cell+ dup >r c@ or r> c! ; 
: IMMEDIATE     $80 flags! ;
: NO-INTERPRET  $40 flags! ;
: COMPILE-ONLY  $c0 flags! ;

: VARIABLE <C> do_variable; addname 0 , ;
