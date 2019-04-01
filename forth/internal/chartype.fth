\ Copyright 2018, by J. Richard Barnette, All Rights Reserved.

: GRAPHIC? ( char -- flag ) 33 127 within ;

: TOUPPER ( char -- uc-char )
    dup [char] a - 26 u< if [ char A char a - ] literal + then
;
