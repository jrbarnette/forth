\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

only FORTH definitions

variable meta-state  0 meta-state !
vocabulary TARGET
: <META> meta-state ! only target ;
: meta[ 1 <META> ;
: ]meta 2 <META> ;
: meta-literal
    meta-state @ 2 = if s" LITERAL" meta-interpret then ;
: meta-immediate align here create name>id , , does> 2@ meta-interpret ;

: handler? ?dup 0= if parse-name then ;

direct: do-name { .exec }{ parse-name .str }{ handler? .exec } ;
direct: linkname { s" i_linkname" .exec } ;
direct: setflags { s" i_setflags" .exec }{ c-hex .cell } ;

: addname s" i_addname" do-name ;
: start-: s" do_colon" s" i_startname" do-name ;

also TARGET definitions previous
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
: [COMPILE] parse-name meta-compile ;
direct: [CHAR] char { s" do_literal" .exec }{ c-hex .cell } meta-literal ;
\ POSTPONE
\ S"
\ [']

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

only FORTH also TARGET definitions DIRECT

\ N.B. "," and "allot" here must come from the DIRECT vocabulary
: CONSTANT s" do_constant" addname , ;
: VARIABLE s" do_variable" addname
    { s" do_literal" .exec }{ s" CELL_SIZE" .cell } allot ;
