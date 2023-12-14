\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

ONLY FORTH definitions

: escape-nul ( #nul -- ) ?dup if 0 do ." \0" loop ." 00" then ;
: escaped '\' emit ;
: escape? ( char quote -- )
       over = if escaped exit then
    dup '\' = if escaped exit then
    dup   7 = if drop escaped 'a' exit then
    dup   8 = if drop escaped 'b' exit then
    dup   9 = if drop escaped 't' exit then
    dup  10 = if drop escaped 'n' exit then
    dup  12 = if drop escaped 'f' exit then
    dup  13 = if drop escaped 'r' exit then
;
: octal-escape ( ctrl-char -- )
    base @ >r 8 base ! 0 <# # # # #> r> base ! escaped type ;
: escape-char ( char quote -- )
    escape? dup printable? if emit else octal-escape then ;
: escape ( #nul c -- #nul )
    ?dup if swap escape-nul [char] " escape-char 0 else 1+ then ;
: escape-string ( c-addr u -- )
    over >r chars + 0 swap r> ?do i c@ escape 1 chars +loop drop ;
: .c-string ( c-addr u -- ) '"' emit escape-string '"' emit ;
: .c-decimal ( u -- ) base @ swap decimal 1 .r base ! ;
: .c-hex ( u -- c-addr u )
    base @ swap hex <# dup >r abs 0 #s 'x' hold '0' hold r> sign #>
    type base ! ;
: .c-char [char] ' dup emit swap over escape-char emit ;
