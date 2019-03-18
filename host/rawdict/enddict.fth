\ Copyright 2015, by J. Richard Barnette, All Rights Reserved.

>>> #include <stddef.h>
>>>
>>> #include "forth.h"
>>> #include "direct.h"
>>>
>>> union {
>>>     cell_ft		cell;
>>>     char		str[sizeof (cell_ft)];
>>>     addr_ft		label;
>>>     vminstr_fn		handler;
>>> } raw_dictionary[] = {

flush-target-dictionary

>>> };
>>>
 .( #define WORDLIST ) lp @ target-dict - 1 .r cr
>>>
>>> DIRECT_FORTH(initialize_forth) // {
>>>         L(raw_dictionary) L(dictionary.dict_space)
>>>         L(sizeof (raw_dictionary)) X(x_move)
>>>
>>>         L(sizeof (raw_dictionary)) L(&DICT.here) X(x_store)
>>>
>>>         L(&dictionary.dict_space[WORDLIST])
>>>         L(&DICT.forth_wordlist) X(x_store)
>>> END_DIRECT // }
