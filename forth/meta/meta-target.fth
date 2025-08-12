\  Copyright 2023, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  ]				METADICT-DEFINERS
\  VARIABLE			METADICT-DEFINERS
\  CONSTANT			METADICT-DEFINERS
\  :				METADICT-DEFINERS
\
\  '				METADICT-HOST
\
\  \				METADICT-TARGET (compile special)
\  (				METADICT-TARGET (compile special)
\  .(				METADICT-TARGET (compile special)
\
\  <C>				METADICT-TARGET (compile special)
\  LITERAL			METADICT-TARGET (compile special)
\  [				METADICT-TARGET (compile special)
\  ;				METADICT-TARGET (compile special)
\
\  ?branch			METADICT-TARGET (augmented primitive)
\  branch			METADICT-TARGET (augmented primitive)
\
\  [']				METADICT-SPECIAL (compile special)
\  [COMPILE]			METADICT-SPECIAL (compile special)
\  POSTPONE			METADICT-SPECIAL (compile special)
\ ------  ------  ------  ------  ------  ------  ------  ------


\ Support for literals when compiling meta-dictionary source files

METADICT-HOST-MODE
: operand>  cell+ dup @ ; compile-only
: cell-operand  operand> { .cell } ;

: do-literal [ also METADICT-TARGET ] do-literal [ previous ] ;

: xt-literal    do-literal operand> execute ; compile-only
: expr-literal  do-literal operand> count { .expr } ; compile-only
: cell-literal  do-literal cell-operand ; compile-only

here 64 chars allot constant expr-buffer
variable expr-ptr

METADICT-DEFINITIONS
: <C>  ';' parse expr-ptr @ dup >r
    ( src u dst ) ( R: dst )
    2dup c! char+ swap chars 2dup + expr-ptr !
    move ['] expr-literal , r> , ; compile-special

: LITERAL ['] cell-literal , , ; compile-special


\ Definitions needed for the implementation of control flow primitives
\ to compile meta=dictionary target sources

also METADICT-TARGET
: branch        branch     cell-operand ; compile-only
: ?branch       ?branch    cell-operand ; compile-only
previous


\ Support for comments when compiling meta-dictionary sources

: \ postpone \ ; immediate
: ( postpone ( ; immediate
: .( postpone .( cr ; immediate


\ Standard defining words implemented for compiling meta-dictionary
\ sources

METADICT-HOST-MODE
: start-name
    target-create 15 spaces ." // " current-name name>string type cr ;

also METADICT-DEFINERS definitions
: VARIABLE
    start-name s" do_variable" { .exec }{ 0 .cell } emit-nl ;
: CONSTANT
    start-name s" do_constant" { .exec }{ .cell } emit-nl ;
: ] metadict-target-wordlist metadict-special-wordlist 2 set-order ] ;
: : start-name s" do_colon" { .exec } emit-nl expr-buffer expr-ptr ! here ] ;


\ Special words implemented for compiling meta-dictionary sources

METADICT-HOST-MODE
: parse-valid-name ( "name" -- nt )
    parse-name metadict-target-wordlist wid-lookup
    dup 0= if -13 .error then ;

: ' ( "name" -- xt )  parse-valid-name name>xt ;

also METADICT-SPECIAL definitions previous
: [COMPILE] ' , ; compile-special
: POSTPONE
    parse-valid-name name>xt+flags immediate? 0=
    if [compile] literal ['] , then compile, ; compile-special
: ['] ' ['] xt-literal , , ; compile-special


METADICT-DEFINITIONS
: [ metadict-target-mode postpone [ ; immediate

also METADICT-SPECIAL
: ;
    postpone EXIT
    here over ( start end cur )
    begin 2dup > while
	dup @ execute emit-nl cell+
    repeat drop - allot postpone [ ; compile-special
