\ Copyright 2018, by J. Richard Barnette, All Rights Reserved.

: GRAPHIC? ( char -- flag ) 33 127 within ;

: TOUPPER ( char -- uc-char )
    dup 'a' - 26 u< if [ 'A' 'a' - ] literal + then
;
