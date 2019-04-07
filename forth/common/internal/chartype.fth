\ Copyright 2018, by J. Richard Barnette, All Rights Reserved.

: GRAPHIC? ( char -- flag ) 33 127 within ;
: PRINTABLE? ( char -- flag ) 32 127 within ;

: CHANGECASE over swap - 26 u< if [ 'A' 'a' xor ] literal xor then ;
: TOUPPER ( char -- uc-char ) 'a' changecase ;
: TOLOWER ( char -- lc-char ) 'A' changecase ;
