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

\ non-standard primitives
\ CLEARSTACK ( i*x -- )
\ CLEARRSTACK ( R: i*x -- )

\ non-standard constants
\ NF-IMMEDIATE		- flag for immediate definitions
\ NF-NOINTERP		- flag for no-interpret definitions

variable STATE
: [ false state ! ; immediate
: ] true  state ! ;

: INTERPRET
    begin parse-word dup while			( str len )
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
    \ save current source specification
    >in   dup @ >r			( R: >IN )
	0 over !			( 0 TO >IN )
    cell+ dup @ >r			( R: >IN SOURCE-ID )
	-1 over !			\ -1 TO SOURCE-ID
    cell+ dup 2@ 2>r			( R: >IN SOURCE-ID c-addr u )
	2!				\ TO SOURCE
    interpret
    \ restore previous source specification
    2r> >in 2 cells + 2!		\ TO SOURCE
    r> >in cell+ !			\ TO SOURCE-ID
    r> >in !
;

: QUIT
    clearrstack 0 >in cell+ !		\ TO SOURCE-ID
    postpone [ begin state @ 0= if ." ok " then
	refill while interpret
    repeat bye
;

: ABORT clearstack quit ;
: ABORT" postpone if postpone ." postpone abort postpone then ; immediate
