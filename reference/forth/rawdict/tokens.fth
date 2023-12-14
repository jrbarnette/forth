\ Copyright 2022, by J. Richard Barnette, All Rights Reserved.
\ Raw dictionary token handling

\ HOST definitions

: INTERPRET-NAME ( i*x name -- j*x )
    \ in (target) compilation state:
    \   if name in target-compile wordlist
    \     execute
    \   if name in target dictionary
    \     name>xt compile,
    \ in interpretation state:
    \   name must not be in target dictionary
    \   name>interpret ?dup if execute else -14 .error then
    ;

: INTERPRET-NUMBER ( x -- | x )
    \ in (target) compilation state
    \   execute LITERAL from target-compile wordlist:
    \     get xt of do-literal in target
    \     [ also host ] compile, , [ previous ]
    ;

: INTERPRET-UNKNOWN ( str len -- )
    state @ if
	link@ wid-lookup ?dup if
	    name>xt compile, true exit
	then
    else
	2drop
    then false
;

: ' parse-name link@ wid-lookup ?dup if name>xt else -13 throw then ;
