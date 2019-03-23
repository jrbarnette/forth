\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

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
direct: [COMPILE] { s" i_compile" .exec }{ parse-name .str } ;
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
: CONSTANT s" do_constant" addname direct-emit , ;
: VARIABLE s" do_variable" addname
    direct-emit { s" do_literal" .exec }{ s" CELL_SIZE" .cell } allot ;
