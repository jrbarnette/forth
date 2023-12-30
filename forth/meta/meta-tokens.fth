\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.
\ Forth text interpreter token handling

META-HOST-MODE definitions

: INTERPRET-NAME ( i*x name -- j*x )
    state @ if
	name>xt+flags immediate? if execute else , then
    else
	name>interpret ?dup if execute else -14 .error then
    then
;


also META-SPECIAL
: INTERPRET-NUMBER ( x -- | x ) state @ if postpone literal then ;
previous


: INTERPRET-UNKNOWN ( str len -- ) false ;
