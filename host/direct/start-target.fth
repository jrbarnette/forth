\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

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

variable offset      0 offset !
variable meta-state  0 meta-state !
variable emit-state  0 emit-state !

: .offset ( n -- ) ." /* " offset @ 3 .r ."  */ " spaces ;
: { ." { " ;
: }{ ."  }, { " 1 offset +! ;
: } ."  }," cr 1 offset +! ;
: emit-state!
    dup emit-state @ <> if
	emit-state @ if 4 .offset { ." .id = NULL" } then
	dup if
	    dup 1- if s" meta_compile" else s" meta_interpret" then
	    0 .offset { .exec }
	then
    then emit-state !
;
: direct-emit ( -- ) 0 emit-state! 0 .offset ;
: meta-emit ( name len state -- ) emit-state! 4 .offset { .str } ;
: meta-interpret ( name len -- ) 1 meta-emit ;
: meta-compile ( name len -- ) 2 meta-emit ;

: direct: : postpone direct-emit ;
direct: setflags { s" i_setflags" .exec }{ c-hex .cell } ;

: handler? ?dup 0= if parse-name then ;
direct: do-name { .exec }{ parse-name .str }{ handler? .exec } ;
: addname s" i_addname" do-name ;
: start-: s" do_colon" s" i_startname" do-name ;
direct: linkname { s" i_linkname" .exec } ;

vocabulary TARGET
vocabulary DIRECT
vocabulary META

: <META> meta-state ! only target ;
: meta[ 1 <META> ;
: ]meta 2 <META> ;
: meta-literal
    meta-state @ 2 = if s" LITERAL" meta-interpret then ;
: meta-immediate align here create name>id , , does> 2@ meta-interpret ;

\ Can't have these be in either FORTH or TARGET search orders
also META definitions
: prim: create parse-name dup , chars here swap dup allot move
     does> direct-emit { dup cell+ swap @ .exec } ;
: IMMEDIATE    ;
: NO-INTERPRET ;
: COMPILE-ONLY ;

only FORTH also TARGET definitions META
: <HOST> only forth ;
: <DIRECT> 0 <META> also direct ;
: \ postpone \ ;
: ( postpone ( ;
: .( postpone .( cr ;
: [ META[ ;
: ] ]META ;

: >>> source >in @ over >in ! swap over - >r chars + r> type cr ;

: LITERAL meta-literal ;
direct: <C> { s" do_literal" .exec }{ [char] ; parse .cell } meta-literal ;
direct: [compile] { s" i_compile" .exec }{ parse-name .str } ;

meta-immediate +LOOP
meta-immediate BEGIN
meta-immediate DO
meta-immediate DOES>
meta-immediate ELSE
meta-immediate IF
meta-immediate LOOP
meta-immediate REPEAT
meta-immediate THEN
meta-immediate UNTIL
meta-immediate WHILE

hex
: IMMEDIATE    80 setflags ;
: NO-INTERPRET 40 setflags ;
: COMPILE-ONLY c0 setflags ;
decimal

: prim: 0 addname ;

: : start-: ]meta ;
: ; s" EXIT" meta-compile linkname meta[ ;

only FORTH also META also DIRECT definitions