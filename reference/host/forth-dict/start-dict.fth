\ Copyright 2015, by J. Richard Barnette, All Rights Reserved.

host-mode

24 1024 * aligned	constant  dictionary-size

create    dictionary	dictionary-size allot
variable  name-ptr	0 name-ptr !

' dictionary-size @	constant mcp-constant
' name-ptr @		constant mcp-variable

dictionary constant here
: allot ( n -- ) [ ' here cell+ ] literal +! ;
: align ( -- ) here aligned here - allot ;
: , ( x -- ) here [ 1 cells ] literal allot ! ;
: c, ( char -- ) here [ 1 chars ] literal allot c! ;

: parse-word begin dup parse dup 0= while 2drop repeat rot drop ;
: link, ( -- new-link ) align here name-ptr @ , ;
: name, ( c-addr u -- ) dup c, here swap chars dup allot move align ;
: create-name ( mcp "<spaces>name" -- new-link )
    link, swap bl parse-word name, ,
;

: link-name ( new-link -- ) name-ptr ! ;
: constant mcp-constant create-name , link-name ;
: variable mcp-variable create-name 1 cells allot link-name ;
: prim: ( "<spaces>name" -- )
    >in @ ' @ swap >in ! create-name link-name
;

hex
: immediate name-ptr @ cell+ dup c@ 80 or swap c! ;
: no-interpret name-ptr @ cell+ dup c@ 40 or swap c! ;
: compile-only name-ptr @ cell+ dup c@ c0 or swap c! ;
decimal
