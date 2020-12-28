\ Copyright 2020, by J. Richard Barnette, All Rights Reserved.

\  literal.fth - Colon-definitions needed to implement LITERAL.

\ Content under the "mem" rubric:
\ ------  ------  ------  ------  ------  ------  ------  ------
\  +!                    6.1.0130 CORE
\ ------  ------  ------  ------  ------  ------  ------  ------

\ Content under the "arith" rubric:
\ ------  ------  ------  ------  ------  ------  ------  ------
\ WITHIN                6.2.2440 CORE EXT
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

\ The standard sequence to perform ","
\   1 cells here-addr <calculate cell> over @ ! +!
\ The standard sequence to compile a literal
\   do-literal [ 1 cells here-addr <calculate cell> over @ ! +! ]

: +! ( x a-addr -- ) swap over @ + swap ! ;
: WITHIN ( x1 x2 x3 -- flag ) over - >r - r> u< ;

\ CONSTANT depends on "," being in the target dictionary, so we have
\ to do it the hard way.
prim: here-addr do_constant
    <C> CELL_SIZE; <C> &HERE; dup dup @ ! +!

: HERE ( -- addr ) here-addr @ ;

\ <sigh> without LITERAL or , or even ALLOT, this is ... tiresome.
\ : allot-bounds <C> DICTSPACE_END+1; here - dup <C> DICTSPACE_SIZE+1; - ;
\ : ALLOT dup allot-bounds within -8 and throw here-addr +! ;

: allot-bounds ( -- unused+1 -inuse )
    do-literal [ <C> DICTSPACE_END; 1 + here !
		 <C> CELL_SIZE; here-addr +! ] here -
    dup do-literal [ <C> DICTSPACE_SIZE; 1 + here !
		     <C> CELL_SIZE; here-addr +! ] - ;
: ALLOT ( n -- ) dup allot-bounds within
    do-literal [ -8 here ! <C> CELL_SIZE; here-addr +! ] and throw
    here-addr +! ;

: , ( x -- ) here do-literal [ <C> CELL_SIZE; here over allot ! ] allot ! ;

: COMPILE, ( xt -- ) , ; compile-only
: LITERAL do-literal do-literal compile, , ; compile-special
