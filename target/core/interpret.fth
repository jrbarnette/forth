\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\ ABORT                 6.1.0670 CORE                   32
\ ABORT"                6.1.0680 CORE                   32
\ EVALUATE              6.1.1360 CORE                   38
\ QUIT                  6.1.2050 CORE                   43
\ STATE                 6.1.2250 CORE                   45
\ [                     6.1.2500 CORE                   48
\ ]                     6.1.2540 CORE                   49
\ ------  ------  ------  ------  ------  ------  ------  ------

\ anonymous definitions
\ clearstack ( i*x -- )
\ clearrstack ( R: i*x -- )
\ require-interpret - signal an error if not in interpretation state
: require-interpret ( -- ) state @    if -29 throw then ;

\ require-compile   - signal an error if not in compilation state
\                     or there is no current definition
: require-compile   ( -- ) state @ 0= if -14 throw then ;

variable STATE
: [ false state ! ; immediate
: ] true  state ! ;

: interpret
    begin parse-name dup while			( str len )
	2dup lookup if				( str len xt flags )
	    \ compile or execute a definition
	    2swap 2drop state @ if
		nf-immediate and if execute else compile, then
	    else
		nf-nointerp and if -14 throw else execute then
	    then
	else					( str len )
	    \ try to convert a number
	    over c@ [char] - = >r		( R: neg? )
	    r@ + swap r@ - swap			\ adjust for sign char
	    0 dup 2swap >number			( ul uh str len )
	    if -13 throw then 2drop		( ul )
	    r> if negate then			( n )
	    state @ if postpone literal then
	then
    repeat
;

: EVALUATE ( i*x c-addr u -- j*x )
    \ current source specification >R
    \ XXX -1 TO SOURCE-ID
    begin [ ' source >body ] literal @ >in @ > while interpret repeat
    \ R> current source specification
;

: QUIT
    clearrsp
    \ XXX 0 TO SOURCE-ID
    POSTPONE [
    BEGIN REFILL WHILE
	interpret STATE @ 0= IF CR ." OK " THEN
    REPEAT BYE
;

: ABORT clearsp QUIT ;
: ABORT" postpone if postpone ." postpone abort postpone then ; immediate
