\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

only FORTH definitions

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
    base @ >r hex
    <# dup >r abs 0 #s [char] x hold [char] 0 hold r> sign #>
    r> base ! ;

: .exec ( c-addr u -- ) ." .handler = " type ;
: .str ( c-addr u -- ) ." .id = " c-string ;
: .expr ( c-addr u -- ) ." .cell = (cell_ft) (" type [char] ) emit ;
: .cell ( u -- ) c-hex .expr ;

variable offset      0 offset !

: { ." { " ;
: }{ ."  }, { " 1 offset +! ;
: } ."  }," cr 1 offset +! ;

: .offset ( n -- ) ." /* " offset @ 3 .r ."  */ " spaces ;

variable emit-state  0 emit-state !
: emit-state!
    dup emit-state @ <> if
	emit-state @ if 4 .offset { ." .id = NULL" } then
	dup if
	    dup 1- if s" meta_compile" else s" meta_interpret" then
	    0 .offset { .exec }
	then
    then dup emit-state ! if 4 else 0 then .offset
;
: direct-emit ( -- ) 0 emit-state! ;
: meta-emit ( name len state -- ) emit-state! { .str } ;

: meta-interpret ( name len -- ) 1 meta-emit ;
: meta-compile ( name len -- ) 2 meta-emit ;
: direct: : postpone direct-emit ;

direct: do-literal { s" do_literal" .exec }{ execute } ;
: direct-expr ['] .expr do-literal ;
: direct-literal ['] .cell do-literal ;

vocabulary DIRECT

\ We only need these while building up the DIRECT vocabulary.
vocabulary BUILD-DIRECT
also BUILD-DIRECT definitions
: prim: create parse-name dup , chars here swap dup allot move
     does> direct-emit { dup cell+ swap @ .exec } ;
: IMMEDIATE    ;
: NO-INTERPRET ;
: COMPILE-ONLY ;

only FORTH also BUILD-DIRECT also DIRECT definitions
