\  Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\ extern union dict {
\     struct {
\ 	addr_ft		here;		    /* HERE */
\ 	name_p		forth_wordlist;	    /* FORTH-WORDLIST */
\
\ 	size_t		lineno;
\ 	FILE *		input;
\     } dict_static_data;
\     addr_unit_ft	dict_space[DICTIONARY_SIZE];
\ } dictionary;

<C> (&DICT) + 1;          <C> &DICT.here;             !
\ forth_wordlist initialized to 0

\ remaining fields initialized to 0.

[
