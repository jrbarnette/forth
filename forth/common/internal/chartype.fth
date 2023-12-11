\ Copyright 2018, by J. Richard Barnette, All Rights Reserved.

: GRAPHIC? ( char -- flag ) 33 127 within ;
: PRINTABLE? ( char -- flag ) 32 127 within ;

: CHANGECASE over swap - 26 u< [ 'A' 'a' xor ] literal and xor ;
: TOUPPER ( char -- uc-char ) 'a' changecase ;
: TOLOWER ( char -- lc-char ) 'A' changecase ;
: STR-TOUPPER ( c-addr u -- )
    0 ?do dup c@ toupper over c! char+ loop drop ;
: STR-TOLOWER ( c-addr u -- )
    0 ?do dup c@ tolower over c! char+ loop drop ;
