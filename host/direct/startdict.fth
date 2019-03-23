\  Copyright 2017, by J. Richard Barnette. All Rights Reserved.

TARGET <DIRECT>

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
1 <C> &DICT.n_search_order;    !
[
