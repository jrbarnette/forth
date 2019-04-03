\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

only FORTH definitions

variable meta-state  0 meta-state !
vocabulary TARGET
: <META> meta-state ! only target ;
: meta[ 1 <META> ;
: ]meta 2 <META> ;
: meta-literal
    meta-state @ 2 = if s" LITERAL" meta-interpret then ;
: meta-char char direct-literal ;
: meta-immediate align here create name>id , , does> 2@ meta-interpret ;

also TARGET definitions previous
: <HOST> only forth ;
: <DIRECT> 0 <META> also direct ;
: \ postpone \ ;
: ( postpone ( ;
: .( postpone .( cr ;

: prim: 0 addname ;

: >>> source >in @ over >in ! swap over - >r chars + r> type cr ;

meta-immediate +LOOP
meta-immediate AGAIN
meta-immediate BEGIN
meta-immediate DO
meta-immediate ELSE
meta-immediate IF
meta-immediate LITERAL
meta-immediate LOOP
meta-immediate REPEAT
meta-immediate THEN
meta-immediate UNTIL
meta-immediate WHILE

: <C> [char] ; parse direct-expr meta-literal ;
: [CHAR] meta-char meta-literal ;
: POSTPONE
    parse-name 2dup 2>r get-order 2r> lookup ?dup if
        name>id meta-compile 2drop
    else
        direct-lookup [ also target ] literal [ previous ]
        s" ," meta-compile
    then
;
: [COMPILE] parse-name meta-compile ;

\ S"
\ [']

\ XXX From here to the end, the definitions we're creating only
\ provide correct behavior in meta-interpret mode.  Generally, to
\ compile the names into the target dictionary you must use
\ [COMPILE]; POSTPONE will do the wrong thing.
\
\ For now, the amount of affected code is small, so it works.
\
\ Eventually, we may need some way to mark definitions as "in
\ compilation state, compile this name, even though it's in the
\ TARGET vocabulary".  Or maybe, we just need a special defining
\ word that makes these definitions change behavior based on
\ meta-state.

: [ META[ ;
: ] ]META ;
: CHAR meta-char ;

hex
: IMMEDIATE    nf-immediate setflags ;
: NO-INTERPRET nf-compile-only setflags ;
: COMPILE-ONLY nf-compile-special setflags ;
decimal

: : s" do_colon" startname ]meta ;
: ; s" EXIT" meta-compile linkname meta[ ;

also DIRECT

\ N.B. "," and "allot" here must come from the DIRECT vocabulary
: CONSTANT s" do_constant" addname , ;
: VARIABLE s" do_variable" addname s" CELL_SIZE" direct-expr allot ;