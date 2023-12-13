\ Copyright 2023, by J. Richard Barnette. All Rights Reserved.
\ Internal definitions relating to namespace lookup and definition.

: ID, ( c-addr u -- ) here >r counted, r> count str-toupper ;

: CURRENT-NAME ( -- name ) get-current @ ;
: LINK-NAME ( name -- ) get-current ! ;

: NAME, ( handler c-addr u -- name )
    align here >r current-name , id, align , r> ;
