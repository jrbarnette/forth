\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.
\ Forth text interpreter token handling

: INTERPRET-NAME ( i*x name -- j*x )
    name>xt+flags state @ if
	immediate? if execute else compile, then
    else
	compile-only? if -14 throw else execute then
    then
;
: INTERPRET-NUMBER ( x -- | x ) state @ if postpone literal then ;
: INTERPRET-UNKNOWN ( str len -- ) type space -13 throw ;
