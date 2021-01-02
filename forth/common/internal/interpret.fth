\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.
\ Definition of INTERPRET, and supporting code
\
\ This code is responsible for parsing and recognizing tokens according
\ to the rules in section 3.4 "The Forth text interpreter":
\  a) A token is parsed, according to the rules in 3.4.1.
\  b) If the token is a name in the dictionary per section 3.4.2,
\     processing calls INTERPRET-NAME.
\  c) If the token can be converted to a number per section 3.4.1.3,
\     processing calls INTERPRET-NUMBER.
\  d) Otherwise, processing calls INTERPRET-UNKNOWN.
\
\ The implementations of INTERPRET-NAME, INTERPRET-NUMBER, and
\ INTERPRET-UNKNOWN are external, and this code may compile in
\ different version depending on the vocabulary context.
\ Specifically, meta-interpretation provides implementations that
\ are different from the prescriptions of the Forth standard.


: 'CHAR' ( c-addr u -- char true | false )
    3 <> if drop false exit then
    char+ dup c@ swap char+
    c@ [char] ' = if true else drop false then ;
: SIGNED-NUMBER ( c-addr u char -- x -1 | 0 )
    '-' = tuck + ?dup 0= if 2drop false exit then
    >r tuck chars - >r
    0 dup r> r> >number if 2drop 2drop false exit then
    2drop swap if negate then true ;
: BASE-SIGNED-NUMBER ( c-addr u base -- x -1 | 0 )
    swap 1- ?dup 0= if 2drop false exit then
    base @ >r >r base ! char+ r> over c@ signed-number r> base ! ;
: NUMBER-TOKEN? ( c-addr u -- x -1 | 0 )
    over c@
    dup '#' = if drop #10 base-signed-number exit then  \ decimal
    dup '$' = if drop $10 base-signed-number exit then  \ hex
    dup '%' = if drop %10 base-signed-number exit then  \ binary
    dup [char] ' = if drop 'char' exit then
    signed-number ;

: INTERPRET-TOKEN
    2dup lookup ?dup if >r 2drop r> interpret-name exit then
    2dup number-token? if >r 2drop r> interpret-number exit then
    interpret-unknown if exit then
    -13 .error
;

: INTERPRET begin parse-name dup while interpret-token repeat 2drop ;
