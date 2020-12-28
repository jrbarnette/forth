\  Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\ extern union dict {
\     struct {
\ 	addr_ft		here;		    /* HERE */
\ 	name_ft		forth_wordlist;	    /* FORTH-WORDLIST */
\
\ 	size_t		lineno;
\ 	FILE *		input;
\     } dict_static_data;
\     addr_unit_ft	dict_space[DICTSPACE_SIZE];
\ } dictionary;

<C> DICTSPACE_START + sizeof (DICT);    <C> &HERE;    !

\ forth_wordlist initialized to 0 by compiler
\ remaining fields initialized prior to entry

[
