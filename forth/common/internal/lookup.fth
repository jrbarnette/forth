\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.
\ Internal definitions relating to namespace lookup.

: ID= ( c-addr1 u1 c-addr2 u2 -- flag )
    rot over <> if drop 2drop false exit then
    0 do
	over i + c@ toupper over i + c@ toupper <> if
	    2drop false unloop exit
	then
    [ 1 chars ] literal +loop 2drop true
;

: WID-LOOKUP ( c-addr u wid -- name | 0 )
    @ begin dup while >r
	2dup r@ name>string id= if 2drop r> exit then
    r> name>prev repeat drop 2drop 0
;

: LOOKUP ( c-addr u -- name | 0 )
    2>r get-order begin dup while 1- swap
	2r@ rot wid-lookup ?dup if >r
	    begin dup while 1- swap drop repeat drop
	    r> 2r> 2drop exit
	then
    repeat 2r> 2drop
;
