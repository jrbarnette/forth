\ Copyright 2020, by J. Richard Barnette, All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\ .S                 15.6.1.0220 TOOLS
\ ?                  15.6.1.0600 TOOLS
\ DUMP               15.6.1.1280 TOOLS
\ SEE                15.6.1.2194 TOOLS
\ WORDS              15.6.1.2465 TOOLS
\ ------  ------  ------  ------  ------  ------  ------  ------

: stack-max ( -- field-width #/line )
    \ first find largest value on stack, by absolute value
    0 depth begin dup 1 > while 
	dup pick abs rot max swap 1-
    repeat drop
    \ count digits, include one more for sign
    1 swap begin swap 1+ swap base @ / dup 0= until drop
    \ entries per line based on 80-column lines
    \ minimum 5 field width, minimum 1 entry per line
    5 max 80 over 1+ / 1 max ;


: .S stack-max 2>r				( R: field #/line )
    0 depth begin dup 1 > while			( # depth )
	over if space then
	dup pick 2r@ drop .r
	swap 1+ dup r@ = if cr drop 0 then	( depth #+1 )
	swap 1-					( #+1 depth-1 )
    repeat drop if cr then 2r> 2drop ;


: ? ( a-addr -- ) @ . cr ;
