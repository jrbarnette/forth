\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

HOST-MODE definitions

variable meta-state  0 meta-state !

vocabulary TARGET
: TARGET-MODE only target definitions ;
: <META> meta-state ! target-mode ;
: meta[ 0 <META> ;
: ]meta -1 <META> ;
: meta-literal
    meta-state @ if s" LITERAL" meta-interpret then ;
: meta-char char literal-cell ;
: meta-immediate align here create name>string , , does> 2@ meta-interpret ;
: meta-' ( c-addr u -- ) direct-lookup meta-literal ;

also TARGET definitions previous
: \ postpone \ ;
: ( postpone ( ;
: .( postpone .( cr ;

: prim: 0 addname ;

meta-immediate +LOOP
meta-immediate ?DO
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

: <C> ';' parse literal-expr meta-literal ;
: handler: parse-name literal-handler meta-literal ;
: [CHAR] meta-char meta-literal ;
: POSTPONE
    parse-name 2dup lookup ?dup if
        name>string meta-compile 2drop
    else
        meta-' s" ," meta-compile
    then
;
: [COMPILE] parse-name meta-compile ;
: ['] parse-name meta-' ;

\ S"

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
: CHAR meta-state @ if s" CHAR" meta-compile else meta-char then ;

: IMMEDIATE        nf-immediate setflags ;
: COMPILE-ONLY     nf-compile-only setflags ;
: COMPILE-SPECIAL  nf-compile-special setflags ;

: : s" do_colon" startname ]meta ;
: ; s" EXIT" meta-compile linkname meta[ ;

: CONSTANT s" do_constant" addname s" ," meta-interpret ;
: VARIABLE s" do_variable" addname
    s" CELL_SIZE" literal-expr s" ALLOT" meta-interpret ;
