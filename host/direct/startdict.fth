\  Copyright 2017, by J. Richard Barnette. All Rights Reserved.

only FORTH also TARGET definitions also DIRECT

\ N.B. "," and "allot" here comes from the DIRECT vocabulary
: CONSTANT s" do_constant" addname , ;
: VARIABLE s" do_variable" addname
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
