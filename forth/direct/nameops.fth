\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\  nameops.fth - Forth code to implement operations relating to
\      creating definitions.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  :                     6.1.0450 CORE                   30
\  ;                     6.1.0460 CORE                   30
\  >BODY                 6.1.0550 CORE                   31
\  CONSTANT              6.1.0950 CORE                   35
\  CREATE                6.1.1000 CORE                   36
\  DOES>                 6.1.1250 CORE                   37
\  IMMEDIATE             6.1.1710 CORE                   40
\  VARIABLE              6.1.2410 CORE                   47
\
\  FORTH-WORDLIST     16.6.1.1595 SEARCH                119
\  GET-CURRENT        16.6.1.1643 SEARCH                119
\  SET-CURRENT        16.6.1.2195 SEARCH                120
\ ------  ------  ------  ------  ------  ------  ------  ------

<C> &DICT.forth_wordlist;
constant FORTH-WORDLIST

: GET-CURRENT <C> &DICT.current; @ ;
: SET-CURRENT <C> &DICT.current; ! ;

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
