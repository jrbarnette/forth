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

: NAME>PREV ( nt -- nt ) @ ;
: NAME>STRING ( nt -- c-addr u ) cell+ count >count ;
: NAME>XT ( nt -- xt ) name>string chars + aligned ;
: NAME>XT+FLAGS ( nt -- xt flags )
    cell+ count dup >r >count chars + aligned r> >flags ;
: NAME>INTERPRET ( nt -- xt | 0 ) name>xt+flags compile-only? if drop 0 then ;
: NAME>COMPILE ( nt -- x xt )
    name>xt+flags immediate? if ['] execute else ['] compile, then ;
: NAME>FIND ( nt -- xt -1 | xt 1 | 0 )
    dup if name>xt+flags immediate? 0= 1 or then ;
: NAME-FLAGS! ( flags nt -- ) cell+ dup >r c@ or r> c! ;
