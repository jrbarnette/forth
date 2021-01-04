\ Copyright 2020, by J. Richard Barnette, All Rights Reserved.

only FORTH also HOST definitions

: INTERPRET-NAME ( i*x name -- j*x ) name>xt execute ;
: INTERPRET-NUMBER ( x -- | x )
    state @ if [ ' do-literal ] literal compile, , then ;
: INTERPRET-UNKNOWN ( str len -- )
    state @ if
	link@ wid-lookup ?dup if name>xt compile, exit then
    then .error -13 throw ;
