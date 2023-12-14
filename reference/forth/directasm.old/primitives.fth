\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ stack primitives
prim: >R            x_to_r
prim: ?DUP          x_question_dup
prim: DEPTH         x_depth
prim: DROP          x_drop
prim: DUP           x_dup
prim: OVER          x_over
prim: R>            x_r_from
prim: R@            x_r_fetch
prim: ROT           x_rot
prim: SWAP          x_swap

prim: 2>R           x_two_to_r
prim: 2R>           x_two_r_from
prim: 2R@           x_two_r_fetch
prim: PICK          x_pick

\ arith primitives
prim: +             x_plus
prim: -             x_minus
prim: 2*            x_two_star
prim: 2/            x_two_slash
prim: <             x_less_than
prim: =             x_equals
prim: >             x_greater_than
prim: AND           x_and
prim: INVERT        x_invert
prim: LSHIFT        x_lshift
prim: NEGATE        x_negate
prim: OR            x_or
prim: RSHIFT        x_rshift
prim: U<            x_u_less
prim: XOR           x_xor

\ mem primitives
prim: !		    x_store     ( x a-addr -- )
prim: @	    	    x_fetch     ( a-addr -- x )
prim: C!	    x_c_store   ( char c-addr -- )
prim: C@	    x_c_fetch   ( c-addr -- char )
prim: FILL	    x_fill      ( c-addr u char -- )
prim: MOVE	    x_move      ( src dst u -- )

\ vm primitives
prim: EXIT	    x_exit
prim: EXECUTE	    x_execute

prim: THROW	    x_throw
prim: DO-LITERAL    do_literal
prim: CLEAR	    x_clear
prim: RCLEAR	    x_rclear

\ control primitives
prim: BRANCH	    do_skip
prim: ?BRANCH	    do_fskip

: <MARK ( -- dest ) tp @ ;
: <RESOLVE ( dest -- ) tp @ - [ 1 cells ] literal / cell>target ;
: >MARK ( -- orig ) tp @ 0 cell>target ;
: >RESOLVE ( orig -- ) tp @ over - [ 1 cells ] literal / swap ! ;

: LITERAL [ also assembler ] do-literal [ previous ] cell>target ;
: <C>
    [ also assembler ] do-literal [ previous ]
    ';' parse-name here counted, expr>target ;

: ref: create-ref does> @ label>target ;
: CODE create-ref does> c" i_call" handler>target @ label>target ;
: END-CODE [ also assembler ] exit [ previous ] ;
\ : [ ONLY FORTH ;
\ : ]CODE ONLY ASSEMBLER ;

also ASSEMBLER
: >BRANCH ( -- orig ) branch >mark ;
: <BRANCH ( dest -- ) branch <resolve ;
: >?BRANCH ( -- orig ) ?branch >mark ;
: <?BRANCH ( dest -- ) ?branch <resolve ;

: BEGIN ( -- dest ) <mark ;
: THEN ( orig -- ) >resolve ;
: IF ( : -- orig ) >?branch ;
: UNTIL ( dest -- ) <?branch ;
: ELSE ( orig1 -- orig2 ) >branch swap then ;
: AGAIN ( dest -- ) <branch ;

: REPEAT ( orig dest -- ) again then ;
: WHILE ( dest -- orig dest ) if swap ;

