\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\ ABORT                 6.1.0670 CORE                   32
\ ABORT"                6.1.0680 CORE                   32
\ EVALUATE              6.1.1360 CORE                   38
\ POSTPONE              6.1.2033 CORE                   43
\ QUIT                  6.1.2050 CORE                   43
\ STATE                 6.1.2250 CORE                   45
\ [                     6.1.2500 CORE                   48
\ ]                     6.1.2540 CORE                   49
\ ------  ------  ------  ------  ------  ------  ------  ------


: INTERPRET
    begin parse-name dup while		( str len )
	2dup lookup if			( str len xt flags )
	    \ compile or execute a definition
	    2swap 2drop state @ if
		nf-immediate and if execute else , then
	    else
		nf-no-interpret and if
		    -14 throw
		else
		    execute
		then
	    then
	else					( str len )
	    \ try to convert a number
	    over c@ [char] '-' = >r		( R: neg? )
	    \ adjust for sign char
	    r@ + swap r@ chars - swap
	    0 dup 2swap >number			( ul uh str len )
	    if -13 throw then 2drop		( ul )
	    r> if negate then			( n )
	    state @ if postpone literal then
	then
    repeat 2drop ;

: EVALUATE			( i*x c-addr u -- j*x )
    \ save current source specification
    >in 2@ 2>r  source 2@ 2>r
    \ make the string the input source
    0 >in ! -1 >in cell+ ! >in [ 2 cells ] literal + 2!
    [ over , ]
    \ restore original source specification
    2r> >in [ 2 cells + ] literal 2!  2r> >in 2!
;

: QUIT
    clearrstack 0 [ >in cell+ ] literal !
    postpone [
    begin state @ 0 = if
	s" ok " refill-prompt !
	then refill while
	    interpret
    repeat cr s" bye" type cr
    0 >r 0 >r ;
