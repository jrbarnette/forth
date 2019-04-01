\ Copyright 2018, by J. Richard Barnette, All Rights Reserved.
\ Internal definitions relating to namespace.

base @ hex
40 constant NF-COMPILE-ONLY
80 constant NF-IMMEDIATE
c0 constant NF-COMPILE-SPECIAL
1f constant NF-LENGTH
e0 constant NF-FLAGS
base !

: NAME>ID ( name -- c-addr u ) cell+ count nf-length and ;
: NAME>XT ( name -- xt ) name>id chars + aligned ;
: NAME>XT+FLAGS ( name -- xt flags )
    cell+ count dup >r nf-length and chars + aligned r> nf-flags and ;
: NAME>FIND ( name -- xt -1 | xt 1 | 0 )
    dup if name>xt+flags nf-immediate and 0= 1 or then ;
: FLAGS! ( flags wid -- ) @ cell+ dup >r c@ or r> c! ;
