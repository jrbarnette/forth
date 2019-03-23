\  Copyright 2017, by J. Richard Barnette. All Rights Reserved.

TARGET <DIRECT>

>>> #include <stddef.h>
>>>
>>> #include "forth.h"
>>> #include "direct.h"
>>>
>>>
>>> DIRECT_FORTH(initialize_forth) // {

\     struct {
\ 	cell_ft		here;		    /* HERE */
\ 	name_p		forth_wordlist;	    /* FORTH-WORDLIST */
\ 	name_p *	current;	    /* CURRENT */
\ 	cell_ft		n_search_order;
\ 	name_p *	search_order[MAX_SEARCH_ORDER];
\
\ 	definition_d	literal_instr;	    /* for LITERAL runtime xt */
\ 	definition_d	skip_instr;	    /* for ELSE runtime xt */
\ 	definition_d	fskip_instr;	    /* for IF runtime xt */
\ 	definition_d	do_instr;	    /* for DO runtime xt */
\ 	definition_d	plus_loop_instr;    /* for +LOOP runtime xt */
\ 	definition_d	does_instr;	    /* for DOES> runtime xt */
\
\ 	cell_ft		state;		    /* STATE */
\ 	cell_ft		base;		    /* BASE */
\
\ 	/* the input source and parse area - 4 cells total */
\ 	cell_ft		to_in;		    /* >IN */
\ 	cell_ft		source_id;	    /* SOURCE-ID */
\ 	string_ft	source;		    /* SOURCE */
\
\ 	char_ft		tib[256];	    /* TIB */
\ 	int		source_max_len;     /* #TIB */
\ 	size_t		lineno;
\ 	FILE *		input;
\     } dict_static_data;

<C> sizeof (DICT);          <C> &DICT.here;             !
\ forth_wordlist initialized to 0
<C> &DICT.forth_wordlist; dup
                            <C> &DICT.current;          !
                            <C> &DICT.search_order[0];  !
\ search order 1..7 initialized to 0
1                           <C> &DICT.n_search_order;   !
<C> do_literal;             <C> &DICT.literal_instr;    !
<C> do_skip;                <C> &DICT.skip_instr;       !
<C> do_fskip;               <C> &DICT.fskip_instr;      !
<C> do_do;                  <C> &DICT.do_instr;         !
<C> do_plus_loop;           <C> &DICT.plus_loop_instr;  !
<C> do_does;                <C> &DICT.does_instr;       !
0                           <C> &DICT.state;            !
10                          <C> &DICT.base;             !
\ remaining fields initialized to 0.

[
