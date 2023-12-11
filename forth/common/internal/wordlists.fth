\ Copyright 2023, by J. Richard Barnette. All Rights Reserved.
\ Internal definitions relating to namespace lookup and definition.

: ID, ( c-addr u -- ) here >r counted, r> count str-toupper ;

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

: CURRENT-NAME ( -- name ) get-current @ ;
: LINK-NAME ( name -- ) get-current ! ;

: NAME, ( handler c-addr u -- name )
    align here >r current-name , id, align , r> ;
