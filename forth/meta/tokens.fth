\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.
\ Forth text interpreter token handling

HOST-MODE definitions

: INTERPRET-NAME ( i*x name -- j*x )
    state @ if
	name>xt+flags immediate? if execute else , then
    else
	name>interpret ?dup if execute else -14 .error then
    then
;


: INTERPRET-NUMBER ( x -- | x ) state @ if [builder] literal then ;

: INTERPRET-UNKNOWN ( str len -- ) false ;
