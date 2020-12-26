\ Copyright 2020, by J. Richard Barnette, All Rights Reserved.

\  literal.fth - Colon-definitions needed to implement LITERAL.

\ Content under the "mem" rubric:
\ ------  ------  ------  ------  ------  ------  ------  ------
\  +!                    6.1.0130 CORE
\ ------  ------  ------  ------  ------  ------  ------  ------

\ Content under the "dict" rubric:
\ ------  ------  ------  ------  ------  ------  ------  ------
\  ,                     6.1.0150 CORE
\  ALLOT                 6.1.0710 CORE
\  HERE                  6.1.1650 CORE
\ ------  ------  ------  ------  ------  ------  ------  ------

\ Content under the "compile" rubric:
\ ------  ------  ------  ------  ------  ------  ------  ------
\  LITERAL               6.1.1780 CORE
\
\  COMPILE,              6.2.0945 CORE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------

\ We need to implement "LITERAL" ASAP during building, because in
\ meta-compile state, several key constructs use it implicitly.
\ This includes "<C>", and even merely using a number literal.
\
\ However "LITERAL" itself has some dependencies that themselves
\ need to compile literals.  Most especially, we need "," in order
\ to compile in a literal.  The net result is that we have to do
\ some fancy footwork below.

: +! ( x a-addr -- ) swap over @ + swap ! ;

\ CONSTANT depends on "," being in the target dictionary, so we have
\ to do it the hard way.
prim: here-addr do_constant
    <C> CELL_SIZE; <C> &HERE; dup dup @ ! +!

: HERE ( -- addr ) here-addr @ ;
: ALLOT ( n -- ) here-addr +! ;

\ "," depends on compiled literals, which we have to bake in the
\ hard way.
: , ( x -- ) here do-literal [ <C> CELL_SIZE; here over allot ! ] allot ! ;

: COMPILE, ( xt -- ) , ; compile-only
: LITERAL do-literal do-literal compile, , ; compile-special
