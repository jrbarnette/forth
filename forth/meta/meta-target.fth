\  Copyright 2023, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  ]				META-DEFINERS
\  VARIABLE			META-DEFINERS
\  CONSTANT			META-DEFINERS
\  :				META-DEFINERS
\
\  '				META-HOST
\
\  \				META-TARGET (compile special)
\  (				META-TARGET (compile special)
\  .(				META-TARGET (compile special)
\
\  <C>				META-TARGET (compile special)
\  LITERAL			META-TARGET (compile special)
\  [				META-TARGET (compile special)
\  ;				META-TARGET (compile special)
\
\  [']				META-SPECIAL (compile special)
\  [COMPILE]			META-SPECIAL (compile special)
\  POSTPONE			META-SPECIAL (compile special)
\
\  ?branch			META-TARGET (augmented primitive)
\  branch			META-TARGET (augmented primitive)
\ ------  ------  ------  ------  ------  ------  ------  ------


META-HOST-MODE definitions
: operand>  cell+ dup @ ; compile-only
: cell-operand  operand> { .cell } ;

\ Must be very careful with the next few lines, so that we don't
\ pick up *unwanted* primitives from the META-TARGET vocabulary.
also META-TARGET
: xt-literal    do-literal operand> [ previous ] execute ; compile-only
also META-TARGET
: expr-literal  do-literal operand> count { .expr } ; compile-only
: cell-literal  do-literal cell-operand ; compile-only

definitions
: branch        branch     cell-operand ; compile-only
: ?branch       ?branch    cell-operand ; compile-only
previous


META-HOST-MODE definitions
here 64 chars allot constant expr-buffer
variable expr-ptr

META-DEFINITIONS
: <C>  ';' parse expr-ptr @ dup >r
    ( src u dst ) ( R: dst )
    2dup c! char+ swap chars 2dup + expr-ptr !
    move ['] expr-literal , r> , ; compile-special

: LITERAL ['] cell-literal , , ; compile-special

: \ postpone \ ; immediate
: ( postpone ( ; immediate
: .( postpone .( cr ; immediate


META-HOST-MODE definitions
: start-name
    target-create 15 spaces ." // " current-name name>string type cr ;

META-HOST-MODE also META-DEFINERS definitions
: VARIABLE
    start-name s" do_variable" { .exec }{ 0 .cell } emit-nl ;
: CONSTANT
    start-name s" do_constant" { .exec }{ .cell } emit-nl ;
: ] meta-target-wordlist meta-special-wordlist 2 set-order ] ;
: : start-name s" do_colon" { .exec } emit-nl expr-buffer expr-ptr ! here ] ;

\ That was the last time we'll want to put META-DEFINERS onto the
\ search-order uncontrolled, because it now contains a special
\ version of `:` that could screw us up if misused.

META-HOST-MODE definitions
: parse-valid-name ( "name" -- nt )
    parse-name meta-target-wordlist wid-lookup
    dup 0= if -13 .error then ;

: ' ( "name" -- xt )  parse-valid-name name>xt ;

also META-SPECIAL definitions previous
: [COMPILE] ' , ; compile-special
: POSTPONE
    parse-valid-name name>xt+flags immediate? 0=
    if [compile] literal ['] , then compile, ; compile-special
: ['] ' ['] xt-literal , , ; compile-special


META-DEFINITIONS
: [ meta-target-mode postpone [ ; immediate

META-COMPILE-MODE
: ;
    postpone EXIT
    here over ( start end cur )
    begin 2dup > while
	dup @ execute emit-nl cell+
    repeat drop - allot postpone [ ; compile-special
