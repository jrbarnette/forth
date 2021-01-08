\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.
\ Internal definitions relating to namespace lookup.

: LOOKUP ( c-addr u -- name | 0 )
    dup 0= if -16 .error then
    2>r get-order begin dup while 1- swap
	2r@ rot wid-lookup ?dup if >r
	    begin dup while 1- swap drop repeat drop
	    r> 2r> 2drop exit
	then
    repeat 2r> 2drop
;
