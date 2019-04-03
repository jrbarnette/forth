\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.
\ Definition of INTERPRET, and supporting code

: ?TRY-NAME ( i*x str len -- i*x -1 | j*x 0 )
    2>r get-order 2r> lookup
    ?dup if name>xt+flags interpret-name false else true then
;

: ?TRY-NUMBER ( str len -- -1 | x 0 )
    base @ >r over c@                       ( str len base? ) ( R: base )
    dup [char] # = if drop 10 true else
    dup [char] $ = if drop 16 true else
        [char] % = if 2 true else false then then then
    ( str len base true | str len false ) ( R: base )
    if base ! 1- swap char+ swap then
    ( str len ) ( R: base )
    ?dup 0= if r> base ! drop true exit then
    over c@ [char] - = swap over +          ( c-addr neg? u' ) ( R: base )
    ?dup 0= if r> base ! drop true exit then
    \ we have at least one digit
    >r swap over chars - >r             ( neg? ) ( R: base u' c-addr' )
    0 dup r> r> >number r> base ! if    ( neg? ul uh c-addr' ) ( R: )
        2drop 2drop true
    else                                ( neg? ul uh c-addr' )
        2drop swap if negate then
        interpret-number false
    then
;

: INTERPRET
    begin parse-name dup while 2>r
        2r@ ?try-name if 2r@ ?try-number if 2r@ interpret-unknown then then
    2r> 2drop repeat 2drop
;
