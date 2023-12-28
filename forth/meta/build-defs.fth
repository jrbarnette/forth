\  Copyright 2023, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  [BUILDER]			HOST (compile special)
\
\  <C>				BUILDER-DEFS (compile special)
\  [				BUILDER-DEFS (compile special)
\  ;				BUILDER-DEFS (compile special)
\  LITERAL			BUILDER-DEFS (compile special)
\  POSTPONE			BUILDER-DEFS (compile special)
\  \				BUILDER-DEFS (compile special)
\  (				BUILDER-DEFS (compile special)
\  .(				BUILDER-DEFS (compile special)
\
\  ?branch			BUILDER-DEFS (augmented primitive)
\  branch			BUILDER-DEFS (augmented primitive)
\  do-literal			BUILDER-DEFS (augmented primitive)
\
\  :				BUILDER-INTERP
\  VARIABLE			BUILDER-INTERP
\  CONSTANT			BUILDER-INTERP
\  ]				BUILDER-INTERP
\ ------  ------  ------  ------  ------  ------  ------  ------


HOST-MODE definitions
: parse-valid-name ( "name" -- nt )
    parse-name builder-defs-wordlist wid-lookup
    dup 0= if -13 .error then ;

also BUILDER-INTERP definitions previous
: ' ( "name" -- xt )  parse-valid-name name>xt ;

also BUILDER-INTERP
' do-literal constant do-literal-xt

HOST-MODE definitions also BUILDER-INTERP
: [BUILDER] ' compile, ; compile-special

also BUILDER-DEFS definitions previous
: [COMPILE] ' , ; compile-special

HOST-MODE also BUILDER-SPECIAL definitions
: POSTPONE
    parse-valid-name name>xt+flags immediate? 0=
    if [compile] literal ['] , then compile, ; compile-special


HOST-MODE definitions
here 64 chars allot constant expr-buffer
variable expr-ptr

: expr-literal
    [builder] do-literal cell+ dup @ count { .expr } ;

also BUILDER-DEFS definitions previous
: <C>  ';' parse chars expr-ptr @ dup >r
    ( src u dst ) ( R: dst )
    2dup c! char+ 2dup + expr-ptr !
    swap move ['] expr-literal , r> , ; compile-special


HOST-MODE definitions
: xt-literal
    [builder] do-literal cell+ dup @ execute ;

HOST-MODE also BUILDER-INTERP
also BUILDER-DEFS definitions previous
: ['] ' ['] xt-literal , , ; compile-special


HOST-MODE also BUILDER-INTERP definitions
: ] builder-defs-wordlist 1 set-order ] ;
: :
    create-builder-def
    15 spaces ." // " current-name name>string type cr
    s" do_colon" { .exec } emit-nl expr-buffer expr-ptr ! here ] ;
previous
: VARIABLE
    create-builder-def s" do_variable" { .exec }{ 0 .cell } emit-nl ;
: CONSTANT
    create-builder-def s" do_constant" { .exec }{ .cell } emit-nl ;

HOST-MODE definitions
: operand-cell  cell+ dup @ { .cell } ;

also BUILDER-DEFS definitions previous
: do-literal  [builder] do-literal  operand-cell ;
: branch      [builder] branch      operand-cell ;
: ?branch     [builder] ?branch     operand-cell ;

: \ postpone \ ; immediate
: ( postpone ( ; immediate
: .( postpone .( cr ; immediate

: [ BUILDER-INTERP-MODE postpone [ ; immediate

also BUILDER-SPECIAL
: ;
    postpone EXIT
    here over ( start end cur )
    \ emit-compiling
    begin 2dup > while
	dup @ execute emit-nl cell+
    repeat drop - allot postpone [ ; compile-special
: LITERAL postpone do-literal , ; compile-special
