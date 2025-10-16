\  Copyright 2025, by J. Richard Barnette. All Rights Reserved.

BUILDER-HOST-MODE

also METADICT-HOST
: [METADICT] ' compile, ; compile-special
previous

variable build-state

: build-initialize
    .start { s" do_colon" .handler } .end
    .start [metadict] meta-initialize .end
    0 build-state ! ;

: .start-indent .start 4 spaces ;
: //ref-name ( nt ref# -- )
    1 over $100 < if 1+ then swap $10 < if 1+ then //name ;
: .ref ( nt ref# -- )  { dup .cell } //ref-name ;
: start-refs ( state -- )
    1- if s" COMPILE" else s" INTERPRET" then .start .meta .end ;
: end-refs  .start-indent { 0 .cell } .end ;

: build-state! ( new-state -- )
    build-state @ over = if drop exit then
    build-state @ if end-refs then
    dup if dup start-refs then
    build-state ! ;

: .start-meta  0 build-state! .start .meta ;

: emit-ref ( nt ref# refmode -- )
    ?dup if
	build-state! .start-indent
    else
	s" REFERENCE" .start-meta
    then .ref .end ;

variable ref-mode
variable nref  1 nref !
: next-ref  nref @ dup 1+ nref ! ;
: create-ref  create next-ref , current-name dup , name>string
    does> 2@ ref-mode @ emit-ref ;

: handler? ( c-addr u | 0 -- c-addr u )  ?dup 0= if parse-name then ;
: do-name  .start-meta { create-ref .id }{ handler? .handler } .end ;
: addname ( c-addr u | 0 -- )  s" ADDNAME" do-name ;
: startname ( c-addr u | 0 -- )  s" STARTNAME" do-name unlink-name ;
: linkname ( c-addr u | 0 -- )  s" LINKNAME" .start-meta .end link-name ;

: build-setflags  s" SETFLAGS" .start-meta { .cell } .end ;

: do-literal  s" DO_LITERAL" .start-meta { execute } .end ;
: literal-expr ( c-addr u -- )  ['] .expr do-literal ;
: literal-cell ( x -- )  ['] .cell do-literal ;
: literal-handler ( c-addr u )  ['] .handler do-literal ;

also BUILDER-SHADOW definitions
: prim: 0 addname ;
