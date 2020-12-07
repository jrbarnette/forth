\  Copyright 2020, by J. Richard Barnette. All Rights Reserved.

: escape '\' emit ;
: escape? ( char quote -- )
       over = if escape exit then
    dup '\' = if escape exit then
    dup   7 = if drop escape 'a' exit then
    dup   8 = if drop escape 'b' exit then
    dup   9 = if drop escape 't' exit then
    dup  10 = if drop escape 'n' exit then
    dup  12 = if drop escape 'f' exit then
    dup  13 = if drop escape 'r' exit then
;
: octal-escape ( ctrl-char -- )
    base @ swap 8 base ! 0 <# # # # #> escape type base ! ;
: escape-char ( char quote -- )
    escape? dup printable? if emit else octal-escape then ;
: escape-nul ( #nul -- ) ?dup if 0 do ." \0" loop ." 00" then ;
: escape-nul? ( #nul c -- #nul )
    ?dup if swap escape-nul [char] " escape-char 0 else 1+ then ;
: escape-string ( c-addr u -- )
    0 tuck ?do over i chars + c@ escape-nul? loop 2drop ;

: .c-char ( char -- ) [char] ' dup emit swap over escape-char emit ;
: .c-string ( c-addr u -- ) '"' emit escape-string '"' emit ;
: .c-decimal ( u -- ) base @ swap decimal 1 .r base ! ;
: .c-hex ( u -- )
    base @ swap hex dup 0< if '-' emit negate then ." 0x" 1 u.r base ! ;
: .c-cell ( c-addr u -- ) ." (cell_ft) (" type ." )" ;

: >>> source >in @ over >in ! swap over - >r chars + r> type cr ;
