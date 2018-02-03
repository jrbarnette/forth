\ Copyright 2018, by J. Richard Barnette, All Rights Reserved.
\ Internal definitions relating to text interpretation.

: INTERPRET-NAME ( xt flags -- )
    state @ if
	nf-immediate and if execute else compile, then
    else
	nf-compile-only and if -14 throw else execute then
    then
;
: INTERPRET-NUMBER ( x -- | x ) state @ if postpone literal then ;
: INTERPRET-UNKNOWN ( str len -- ) type space -13 throw ;

: EVAL-NAME ( str len -- -1 | x ... 0 )
    2>r get-order 2r> lookup if interpret-name false else true then
;

: EVAL-NUMBER ( str len -- -1 | x? 0 )
    base @ >r over c@                       ( str len ) ( R: base str len )
    dup [char] # = if drop 10 true else
    dup [char] $ = if drop 16 true else
        [char] % = if 2 true else false then then then
    if base ! 1- swap char+ swap then
    over c@ [char] - =                      ( c-addr u neg? )
    swap over + >r swap over chars - >r     ( neg? ) ( R: u' c-addr' )
    0 dup r> r> >number if                  ( neg? ul uh c-addr' )
        2drop 2drop true
    else                                    ( neg? ul uh c-addr' )
        2drop swap if negate then
	interpret-number false
    then r> base !
;

: INTERPRET
    begin parse-name dup while 2>r
        2r@ eval-name if 2r@ eval-number if 2r@ interpret-unknown then then
    2r> 2drop repeat 2drop
;
