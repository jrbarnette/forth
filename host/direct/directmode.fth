\  Copyright 2017, by J. Richard Barnette. All Rights Reserved.

vocabulary DIRECT-ASSEMBLER
only forth also direct-assembler definitions

: escape-nul ( #nul -- ) ?dup if 0 do ." \0" loop ." 00" then ;
: escape-graphic ( c -- )
    dup [char] \ = over [char] " = or if [char] \ emit then emit ;
: escape-non-graphic ( c -- )
    base @ >r 8 base ! 0 <# # # # [char] \ hold #> type r> base ! ;
: escape-char ( c -- )
    dup graphic? if escape-graphic else escape-non-graphic then ;
: escape ( #nul c -- #nul )
    ?dup if swap escape-nul escape-char 0 else 1+ then ;
: escape-string ( c-addr u -- )
    over >r chars + 0 swap r> do i c@ escape 1 chars +loop drop ;
: c-string ( c-addr u -- ) [char] " emit escape-string [char] " emit ;
: c-hex ( u -- c-addr u )
    base @ >r hex 0 <# #s [char] x hold [char] 0 hold #> r> base ! ;

: .exec ( c-addr u -- ) ." .handler = " type ;
: .str ( c-addr u -- ) ." .id = " c-string ;
: .cell ( c-addr u -- ) ." .cell = (cell_ft) (" type [char] ) emit ;

variable offset  0 offset !

: { ( n -- ) ." /* " offset @ 3 .r ."  */" spaces ." { " ;
: }{ ."  }, { " 1 offset +! ;
: } ."  }," cr 1 offset +! ;

variable meta-state  0 meta-state !
variable emit-state  0 emit-state !
: {
    meta-state @ emit-state @ <> if
	emit-state @ if 5 { ." .id = NULL" } then
	meta-state @ dup emit-state !
	?dup if
	    1 { 1- if s" meta_compile" else s" meta_interpret" then .exec }
	then
    then meta-state @ if 5 else 1 then {
;

: prim: create parse-name dup , chars here swap dup allot move
     does> { dup cell+ swap @ .exec } ;
: setflags { s" i_setflags" .exec }{ c-hex .cell } ;
: [compile] { s" i_compile" .exec }{ parse-name .str } ;
: literal { s" do_literal" .exec }{ c-hex .cell } ;
: <C> { s" do_literal" .exec }{ [char] ; parse .cell } ;

: IMMEDIATE    ;
: NO-INTERPRET ;
: COMPILE-ONLY ;

: >>> source >in @ over >in ! swap over - >r chars + r> type cr ;
