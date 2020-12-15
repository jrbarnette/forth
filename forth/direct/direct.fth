\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

only FORTH definitions

variable offset      0 offset !

: .indent ( state -- ) if 4 else 0 then spaces ;
: .offset ( state -- ) ." /* " offset @ 4 .r ."  */" .indent ;

: { ."  { " ;
: } ."  }," 1 offset +! ;
: }{ } { ;

variable emit-state  0 emit-state !

: } } cr ;
: start-instr ( new-state -- )
    dup emit-state @ <> if
	emit-state @ ?dup if .offset { 0 .str } then
	dup if
	    dup 1- if s" meta_compile" else s" meta_interpret" then
	    0 .offset { .exec }
	then dup emit-state !
    then .offset
;

: meta-emit ( name len state -- ) start-instr { .str } ;
: meta-interpret ( name len -- ) 1 meta-emit ;
: meta-compile ( name len -- ) 2 meta-emit ;

: { 0 start-instr { ;

: handler? ( c-addr u | 0 -- c-addr u ) ?dup 0= if parse-name then ;
: do-name { .exec }{ parse-name .str }{ handler? .exec } ;
: addname ( c-addr u | 0 -- ) s" i_addname" do-name ;
: startname ( c-addr u | 0 -- ) s" i_startname" do-name ;
: linkname ( -- ) { s" i_linkname" .exec } ;
: setflags ( flags -- ) { s" i_setflags" .exec }{ .cell } ;
: direct-lookup ( c-addr u -- ) { s" i_lookup" .exec }{ .str } ;

\ N.B. do-literal isn't _unnecessary_ complexity.  Both .offset and
\ .c-hex use pictured string formatting.  Thus, the following
\ sequence breaks because { will plaster the output from c-hex:
\     .c-hex { s" do_literal" .exec }{ .expr }
: do-literal { s" do_literal" .exec }{ execute } ;
: literal-expr ['] .expr do-literal ;
: literal-cell ['] .cell do-literal ;
: literal-handler ['] .exec do-literal ;

vocabulary DIRECT

\ We only need these while building up the DIRECT vocabulary.
vocabulary BUILD-DIRECT
also BUILD-DIRECT definitions
: prim: create parse-name counted, does> { count .exec } ;
: IMMEDIATE    ;
: COMPILE-ONLY ;
: COMPILE-SPECIAL ;

only FORTH also BUILD-DIRECT also DIRECT definitions
