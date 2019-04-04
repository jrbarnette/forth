\ Copyright 2018, by J. Richard Barnette, All Rights Reserved.
\ Internal definitions relating to namespace layout.

$40 constant NF-COMPILE-ONLY
$80 constant NF-IMMEDIATE
$c0 constant NF-COMPILE-SPECIAL
$1f constant NF-LENGTH
$e0 constant NF-FLAGS

: IMMEDIATE? ( flags+len -- imm? ) nf-immediate and ;
: COMPILE-ONLY? ( flags+len -- imm? ) nf-compile-only and ;
: >COUNT ( flags+len -- len ) nf-length and ;
: >FLAGS ( flags+len -- flags ) nf-flags and ;

: NAME>PREV ( name -- name ) @ ;
: NAME>ID ( name -- c-addr u ) cell+ count >count ;
: NAME>XT ( name -- xt ) name>id chars + aligned ;
: NAME>XT+FLAGS ( name -- xt flags )
    cell+ count dup >r >count chars + aligned r> >flags ;
: NAME>FIND ( name -- xt -1 | xt 1 | 0 )
    dup if name>xt+flags immediate? 0= 1 or then ;
: NAME-FLAGS! ( flags name -- ) cell+ dup >r c@ or r> c! ;
