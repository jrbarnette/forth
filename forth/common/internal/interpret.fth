\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.
\ Definition of INTERPRET, and supporting code

: NAME-TOKEN? ( i*x str len -- i*x -1 | j*x 0 ) 2>r get-order 2r> lookup ;

: NUMBER-BASE
    dup [char] # = if drop true #10 exit then \ decimal
    dup [char] $ = if drop true $10 exit then \ hex
	[char] % = if      true %10 exit then \ binary
    false base @ ;

: NUMBER-END? swap over + ?dup 0= ;
: BASE? number-base base ! number-end? ;
: SIGN? [char] - = number-end? ;
: NUMBER? >number >r 2drop r> ;
: BASE-SIGN-NUMBER
    over c@ base? if 2drop false exit then >r chars - r>
    over c@ sign? if 2drop false exit then >r tuck chars - >r
    0 dup r> r> number? if 2drop false exit then
    swap if negate then true ;

: NUMBER-TOKEN? base @ >r base-sign-number r> base ! ;

: INTERPRET-TOKEN
    2dup name-token? ?dup if >r 2drop r> interpret-name exit then
    2dup number-token? if >r 2drop r> interpret-number exit then
    interpret-unknown
;

: INTERPRET begin parse-name dup while interpret-token repeat 2drop ;
