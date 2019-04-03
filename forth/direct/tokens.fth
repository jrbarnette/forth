\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.
\ Meta-interpreter token handling

only FORTH definitions

: INTERPRET-NAME ( i*x xt flags -- j*x ) drop execute ;

: INTERPRET-NUMBER ( x -- ) direct-literal meta-literal ;

: INTERPRET-UNKNOWN ( str len -- )
    meta-state @ if
	meta-state @ meta-emit
    else
	type space -13 throw
    then
;
