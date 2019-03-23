\ Copyright 2018, by J. Richard Barnette, All Rights Reserved.
\ Internal definitions relating to text interpretation.

only FORTH definitions

: INTERPRET-NAME ( i*x xt flags -- j*x ) drop execute ;

: INTERPRET-NUMBER ( x -- | x )
    direct-emit { s" do_literal" .exec }{ c-hex .cell } meta-literal ;

: INTERPRET-UNKNOWN ( str len -- )
    meta-state @ if
	meta-state @ meta-emit
    else
	type space -13 throw
    then
;

: ?TRY-NAME ( i*x str len -- i*x -1 | j*x 0 )
    2>r get-order 2r> lookup
    ?dup if name>xt+flags interpret-name false else true then
;

: ?TRY-NUMBER ( str len -- -1 | x? 0 )
    base @ >r over c@                       ( str len sign? ) ( R: base )
    dup [char] # = if drop 10 true else
    dup [char] $ = if drop 16 true else
        [char] % = if 2 true else false then then then
    if base ! 1- swap char+ swap then
    over c@ [char] - =                      ( c-addr u neg? )
    swap over + >r swap over chars - >r     ( neg? ) ( R: base u' c-addr' )
    0 dup r> r> >number if                  ( neg? ul uh c-addr' )
        2drop 2drop true
    else                                    ( neg? ul uh c-addr' )
        2drop swap if negate then
	interpret-number false
    then r> base !
;

: INTERPRET
    begin parse-name dup while 2>r
        2r@ ?try-name if 2r@ ?try-number if 2r@ interpret-unknown then then
    2r> 2drop repeat 2drop
;

: COMPILE-INPUT begin interpret refill 0= until ;

COMPILE-INPUT