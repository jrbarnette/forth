\ Copyright 2020, by J. Richard Barnette, All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\ .S                 15.6.1.0220 TOOLS
\ ?                  15.6.1.0600 TOOLS
\ DUMP               15.6.1.1280 TOOLS
\ SEE                15.6.1.2194 TOOLS
\ WORDS              15.6.1.2465 TOOLS
\ ------  ------  ------  ------  ------  ------  ------  ------

: stack-max
    0 depth begin dup 1 > while 
	dup pick abs rot max swap 1-
    repeat drop
    dup        $10 < if drop  20  2 exit then
    dup       $100 < if drop  16  3 exit then
    dup     $10000 < if drop  12  5 exit then
        $100000000 < if        8  9
		     else      4 17 then ;

\ this is hard-coded for hex; we should do better...
: .S stack-max base @ >r 2>r hex	( R: base #/line width )
    0 depth begin dup 1 > while		( # depth )
	over if space then
	dup pick 2r@ rot swap .r
	rot 1+ swap over = if cr drop 0 then	( depth #+1 )
	swap 1-					( #+1 depth-1 )
    repeat drop if cr then 2r> 2drop r> base ! ;

: ? ( a-addr -- ) @ . cr ;
