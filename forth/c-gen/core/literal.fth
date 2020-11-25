\ Copyright 2020, by J. Richard Barnette, All Rights Reserved.

\  literal.fth - Colon-definitions needed to implement LITERAL.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  +!                    6.1.0130 CORE
\  ,                     6.1.0150 CORE
\  ALLOT                 6.1.0710 CORE
\  HERE                  6.1.1650 CORE
\  LITERAL               6.1.1780 CORE
\
\  COMPILE,              6.2.0945 CORE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------

\ This file is a combination of definitions that would nominally be
\ categorized as "dict", "mem" and "compile" in the index.  They're
\ joined in one file because of the thicket of dependencies around
\ "LITERAL":
\   + "LITERAL" depends on ",".
\   + Stuff like ": CHAR+ 1 + ;" depends on LITERAL implicitly.


\ CONSTANT depends on ",". So, we have to do it the hard way.
prim: here-addr do_constant
<C> &HERE; dup dup @ !
<C> CELL_SIZE; over @ + swap !

\ ALLOT uses this, below.
: +! ( x a-addr -- ) dup >r @ + r> ! ;

: HERE ( -- addr ) here-addr @ ;
: ALLOT ( n -- ) here-addr +! ;

\ "," depends on compiled literals, which we have to bake in the
\ hard way.
: , ( x -- ) here do-literal [
	<C> CELL_SIZE; <C> &HERE; over over @ !
	swap over @ + swap !
    ] allot ! ;

: COMPILE, ( xt -- ) , ; compile-only
: LITERAL do-literal do-literal compile, , ; compile-special
