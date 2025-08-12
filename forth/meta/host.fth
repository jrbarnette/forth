\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

HOST-MODE

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
	    dup 1- if s" META_COMPILE" else s" META_INTERPRET" then
	    0 .offset { .meta }
	then dup emit-state !
    then .offset
;


wordlist constant TARGET-NAMES
variable nreference  1 nreference !
: create-reference
    align here get-current target-names set-current
    nreference @ constant 1 nreference +!
    set-current name>string
;


: .ref ( name len -- )
    2dup ." /* " type ."  */ " 8 over - spaces
    target-names wid-lookup
    dup 0= if cr -13 .error then
    name>xt execute .cell
;

: meta-emit ( name len state -- ) start-instr { .ref } ;
: meta-interpret ( name len -- ) 1 meta-emit ;
: meta-compile ( name len -- ) 2 meta-emit ;

: { 0 start-instr { ;

: handler? ( c-addr u | 0 -- c-addr u ) ?dup 0= if parse-name then ;
: do-name { .meta }{ create-reference .str }{ handler? .exec } ;
: addname ( c-addr u | 0 -- ) s" META_ADDNAME" do-name ;
: startname ( c-addr u | 0 -- ) s" META_STARTNAME" do-name ;
: linkname ( -- ) { s" META_LINKNAME" .meta } ;
: setflags ( flags -- ) { s" META_SETFLAGS" .meta }{ .cell } ;
: direct-lookup ( c-addr u -- ) { s" META_REFERENCE" .meta }{ .ref } ;

\ N.B. do-literal isn't _unnecessary_ complexity.  Both .offset and
\ .c-hex use pictured string formatting.  Thus, the following
\ sequence breaks because { will plaster the output from .c-hex:
\     .c-hex { s" DO_LITERAL" .meta }{ .expr }
: do-literal { s" DO_LITERAL" .meta }{ execute } ;
: literal-expr ( c-addr u -- ) ['] .expr do-literal ;
: literal-cell ( x -- ) ['] .cell do-literal ;
: literal-handler ( c-addr u ) ['] .exec do-literal ;

: meta-init ( -- ) { s" do_colon" .exec }{ s" META_INITIALIZE" .meta } ;
: meta-exit ( -- ) { s" EXIT" .meta } ;
