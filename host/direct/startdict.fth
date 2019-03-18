\  Copyright 2017, by J. Richard Barnette. All Rights Reserved.

<HOST> also forth definitions previous

: handler ?dup 0= if parse-name then ;
: do-name { .exec }{ parse-name .str }{ handler .exec } ;
: addname s" i_addname" do-name ;
: start-: s" do_colon" s" i_startname" do-name ;
: linkname { s" i_linkname" .exec } ;

<HOST> definitions
hex
: IMMEDIATE    80 setflags ;
: NO-INTERPRET 40 setflags ;
: COMPILE-ONLY c0 setflags ;
decimal

: prim: 0 addname ;

also direct
: constant s" do_constant" addname , ;
: variable s" do_variable" addname
    { s" do_literal" .exec }{ s" CELL_SIZE" .cell } allot ;

<DIRECT>
>>> #include <stddef.h>
>>>
>>> #include "forth.h"
>>> #include "direct.h"
>>>
>>>
>>> DIRECT_FORTH(initialize_forth) // {

<C> sizeof (DICT); <C> &DICT.here;     !
<C> &DICT.forth_wordlist; dup
    <C> &DICT.current;                 !
    <C> &DICT.search_order[0];         !
1 literal <C> &DICT.n_search_order;    !
