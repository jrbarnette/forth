\  Copyright 2017, by J. Richard Barnette. All Rights Reserved.

only FORTH definitions

: COMPILE-INPUT begin interpret refill 0= until ;

COMPILE-INPUT
\ Starting with this line, input is processed by the meta-interpreter.
TARGET <DIRECT>

>>> #include <stddef.h>
>>>
>>> #include "forth.h"
>>> #include "direct.h"
>>>
>>>
>>> DIRECT_FORTH(initialize_forth) // {

\   struct {
\	cell_ft		here;		    /* HERE */
\	name_p		forth_wordlist;	    /* FORTH-WORDLIST */
\	name_p *	current;	    /* CURRENT */
\	cell_ft		n_search_order;
\	name_p *	search_order[MAX_SEARCH_ORDER];
\
\	size_t		lineno;
\	FILE *		input;
\   } dict_static_data;

<C> sizeof (DICT);          <C> &DICT.here;             !
\ forth_wordlist initialized to 0
<C> &DICT.forth_wordlist; dup
                            <C> &DICT.current;          !
                            <C> &DICT.search_order[0];  !
\ search order 1..7 initialized to 0
1                           <C> &DICT.n_search_order;   !
\ remaining fields initialized to 0.

[
