\ Copyright 2023, by J. Richard Barnette, All Rights Reserved.
\
\ Parse a number per the rules in section 3.4.1.3 of the standard.


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
