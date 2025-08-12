\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.
\ Meta-interpreter token handling

HOST-MODE

: INTERPRET-NAME ( i*x name -- j*x ) name>xt execute ;

: INTERPRET-NUMBER ( x -- ) literal-cell meta-literal ;

: INTERPRET-UNKNOWN ( str len -- )
    meta-state @ negate 1+ meta-emit true ;
