\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.

: EVALUATE			( i*x c-addr u -- j*x )
    \ save current source specification
    >in @ source-id 2>r source 2>r
    \ make the string the input source
    source! -1 source-id! 0 >in ! interpret
    \ restore original source specification
    2r> source! 2r> source-id! >in !
;
