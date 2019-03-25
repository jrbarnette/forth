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

also TARGET definitions previous
: <HOST> only forth ;
: <DIRECT> 0 <META> also direct ;
: \ postpone \ ;
: ( postpone ( ;
: .( postpone .( cr ;
: [ META[ ;
: ] ]META ;

: prim: 0 addname ;

: >>> source >in @ over >in ! swap over - >r chars + r> type cr ;

: <C> [char] ; parse direct-expr meta-literal ;
: [COMPILE] parse-name meta-compile ;
: [CHAR] char direct-literal meta-literal ;
\ POSTPONE
\ S"
\ [']

meta-immediate +LOOP
meta-immediate BEGIN
meta-immediate DO
meta-immediate DOES>
meta-immediate ELSE
meta-immediate IF
meta-immediate LITERAL
meta-immediate LOOP
meta-immediate REPEAT
meta-immediate THEN
meta-immediate UNTIL
meta-immediate WHILE

\ XXX From here to the end, the definitions we're creating only
\ provide correct behavior in meta-interpret mode.
\
\ For now, nothing needs to compile these names, so it works.
\
\ Eventually, we may need some way to mark definitions as "in
\ compilation state, compile this name, even though it's in the
\ TARGET vocabulary".  Or maybe, we just need a special defining
\ word that makes these definitions change behavior based on
\ meta-state.
hex
: IMMEDIATE    80 setflags ;
: NO-INTERPRET 40 setflags ;
: COMPILE-ONLY c0 setflags ;
decimal

: : s" do_colon" startname ]meta ;
: ; s" EXIT" meta-compile linkname meta[ ;

also DIRECT

\ N.B. "," and "allot" here must come from the DIRECT vocabulary
: CONSTANT s" do_constant" addname , ;
: VARIABLE s" do_variable" addname s" CELL_SIZE" direct-expr allot ;
