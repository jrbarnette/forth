\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

HOST-MODE definitions

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
	emit-state @ ?dup if .offset { 0 .cell } then
	dup if
	    dup 1- if s" meta_compile" else s" meta_interpret" then
	    0 .offset { .exec }
	then dup emit-state !
    then .offset
;


: /*name*/ ( name -- )
    ." /* " name>string dup >r type 15 r> - spaces ."  */ " ;

: create-in-wordlist ( creator-xt wid "name" -- )
    get-current >r set-current execute r> set-current ;

vocabulary META-DEFS
' meta-defs >body constant META-DEFS-WORDLIST

: create-meta-def ( offset -- )
    create current-name , , does> 2@ ( offset name )
	0 start-instr { /*name*/ .label }
;

: create-meta-def ( offset -- )
    ['] create-meta-def meta-defs-wordlist create-in-wordlist ;

wordlist constant REFERENCES
variable nreference  1 nreference !

: create-reference
    >in @ nreference @ dup 1+ nreference !
    ['] constant references create-in-wordlist >in !
    dup 0= if
	>in @ offset @ 1+ create-meta-def >in !
    then parse-name
;

: .ref ( name len -- )
    references wid-lookup dup 0= if cr -13 .error then
    dup /*name*/ name>xt execute .cell
;

: meta-emit ( name len state -- ) start-instr { .ref } ;
: meta-interpret ( name len -- ) 1 meta-emit ;
: meta-compile ( name len -- ) 2 meta-emit ;

: { 0 start-instr { ;

: handler? ( c-addr u | 0 -- c-addr u ) ?dup 0= if parse-name then ;
: do-name { .exec }{ create-reference .str }{ handler? .exec } ;
: addname ( c-addr u | 0 -- ) s" i_addname" do-name ;
: startname ( c-addr u | 0 -- ) s" i_startname" do-name ;
: linkname ( -- ) { s" i_linkname" .exec } ;
: setflags ( flags -- ) { s" i_setflags" .exec }{ .cell } ;
: direct-lookup ( c-addr u -- ) { s" i_reference" .exec }{ .ref } ;

\ N.B. do-literal isn't _unnecessary_ complexity.  Both .offset and
\ .c-hex use pictured string formatting.  Thus, the following
\ sequence breaks because { will plaster the output from .c-hex:
\     .c-hex { s" do_literal" .exec }{ .expr }
: do-literal { s" do_literal" .exec }{ execute } ;
: literal-expr ( c-addr u -- ) ['] .expr do-literal ;
: literal-cell ( x -- ) ['] .cell do-literal ;
: literal-handler ( c-addr u ) ['] .exec do-literal ;
