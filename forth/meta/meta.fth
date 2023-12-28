\ We can create this variable, but initialization has to be done by
\ invoking a defintion; see meta-initialize, below.

variable ref-index

: ref-addr ( index -- a-addr ) cells <C> &reference_table[0]; + ;
: ref@ ( index -- xt ) ref-addr @ ;
: ref! ( a-addr index -- ) ref-addr ! ;
: next-ref! ( name -- ) name>xt ref-index @ tuck ref! 1+ ref-index ! ;

\ meta-mode dictionary building operations:
: meta-initialize
    1 ref-index !
    <C> DICTSPACE_START + sizeof (DICT); <C> &HERE; !
    <C> &FORTH_WORDLIST; set-current
;

: strlen dup begin dup c@ while char+ repeat swap - ;
: startname ( a-addr -- name a-addr' )
    dup 2@ dup strlen name, dup next-ref! swap cell+ cell+ ;
: meta-startname ( -- name ) r> startname >r ;
: meta-addname ( -- ) r> startname >r link-name ;
: meta-setflags ( -- ) r> dup @ current-name name-flags! cell+ >r ;

: meta-interpret ( x*i -- x*j )
    begin r> dup cell+ >r @ ?dup while ref@ execute repeat ;

: meta-compile ( -- )
    r> begin dup cell+ swap @ ?dup while ref@ compile, repeat >r ;

: meta-reference ( -- ref ) r> dup @ ref@ swap cell+ >r ;
