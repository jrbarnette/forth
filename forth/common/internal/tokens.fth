\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.
\ Forth text interpreter token handling

: INTERPRET-NAME ( i*x name -- j*x )
    state @ if
	name>compile execute
    else
	name>interpret ?dup if execute else -14 throw then
    then
;
: INTERPRET-NUMBER ( x -- | x ) state @ if postpone literal then ;
: INTERPRET-UNKNOWN ( str len -- ) false ;
