\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.

vocabulary TARGET
also target definitions

\ We accumulate the target dictionary into a local buffer that we
\ flush at the end of processing.
\
\ The buffer is needed for cases where we backpatch content, as for
\ forward branches (e.g. IF, WHILE) and IMMEDIATE.
\
\ The buffer has the following parts:
\   target-dict		The actual cells of the target dictionary
\   tag-buffer		Tags for .entry, packed as bit fields
\   hdlrs-buffer	Storage for handler-names for .handler

24 1024 *			constant  #target-dict
#target-dict cells		create    target-dict  allot

2				constant  #tag-bits
1 #tag-bits lshift 1-		constant  tag-mask
1 cells 8 * #tag-bits /		constant  #tags/cell

#target-dict #tags/cell + 1-
#tags/cell / cells		create   tag-buffer allot
4096 chars			create   hdlrs-buffer allot

variable  dp	target-dict dp !
variable  hnp	hdlrs-buffer hnp !
variable  lp	0 lp !

: tag-addr ( a-addr -- shift a-addr )
    target-dict - 1 cells /
    #tags/cell /mod >r #tag-bits * r> cells tag-buffer +
;
: tag@ ( a-addr -- tag ) tag-addr @ swap rshift tag-mask and ;
: tag! ( tag a-addr -- ) tag-addr >r lshift r@ @ or r> ! ;

\ .entry is a defintion that will print the value of a target
\ dictionary cell, given a pointer to the cell in the buffer and the
\ tag.

: escape-nul ( #nul -- ) ?dup if 0 do ." \0" loop ." 00" then ;
: escape-graphic ( c -- )
    dup [char] \ = over [char] " = or if [char] \ emit then emit ;
: escape-non-graphic ( c -- )
    base @ swap 8 base ! 0 <# # # # [char] \ hold #> type base ! ;
: escape-non-nul ( c -- )
    dup graphic? if escape-graphic else escape-non-graphic then ;
: escape ( #nul c -- #nul )
    ?dup if swap escape-nul escape-non-nul 0 else 1+ then ;
: escape-string ( c-addr u -- )
    over >r chars + 0 swap r> do i c@ escape 1 chars +loop drop ;
: c-string ( c-addr u -- ) [char] " emit escape-string [char] " emit ;
: c-decimal ( u -- ) base @ swap decimal 1 .r base ! ;
: c-hex ( u -- c-addr u ) base @ swap hex ." 0x" 1 .r base ! ;

: .{ ( -- ) ."   { " ;
: }, ( -- ) ."  }," ;

: .cell ( a-addr -- )  .{ ." .cell = " @ c-hex }, ;
: .str ( a-addr -- )   .{ ." .str = " 1 cells c-string }, ;
: .label ( a-addr -- )
    @ target-dict -
    .{ ." .label = &dictionary.dict_space[" c-decimal ." ]" }, ;
: .handler ( a-addr -- ) .{ ." .handler = " @ count type }, ;

here
    ' .cell ,		\ tag #0
    ' .str ,		\ tag #1
    ' .label ,		\ tag #2
    ' .handler ,	\ tag #3
: .entry ( a-addr tag -- ) cells [ swap ] literal + @ execute ;


: here dp @ ;
\ must be careful now; we have two versions of HERE

: mark-string  1 here 1 cells - tag! ;
: mark-label   2 here 1 cells - tag! ;
: mark-handler 3 here 1 cells - tag! ;
: mark-string? here dup aligned <> if 1 here tag! then ;


\ target dictionary operations
\   HERE ALIGN ALLOT , C,
\   str, handler, label,
\   prim: handler: ref:

: align ( -- ) mark-string? here aligned dp ! ;
: allot ( n -- )
    here + dup aligned here aligned <> if mark-string? then dp !
;
: , ( x -- ) here 1 cells allot ! ;
: c, ( char -- ) here 1 chars allot c! ;
: str, ( c-addr u -- )
    align here >r dup c, here swap chars dup allot move
    here r> begin 2dup u> while 1 over tag! cell+ repeat 2drop
;
: handler, ( c-addr -- ) align , mark-handler ;
: label, ( addr -- ) align dup , if mark-label then ;


: flush-target-dictionary
    decimal
    align here target-dict begin 2dup > while
	dup target-dict - ." /* " 5 .r ."  */"
	dup dup tag@ .entry cr
    cell+ repeat 2drop
;

: >>> source >in @ over >in ! swap over - >r chars + r> type cr ;

: link, ( -- ) align lp @ label, ;
: link-name ( name -- ) lp ! ;

: handler: ( "<name>" -- )
    parse-name					( c-addr u )
    dup hnp @ dup handler, c!			( c-addr u )
    hnp @ char+ swap chars dup >r move		( ) ( R: nchars )
    r> char+ hnp +!				( )
;

: prim: here link, link-name parse-name str, handler: ;
: compile, label, ;

: ref: create here [ only forth ] , immediate does> @ [ also target ] label, ;

: IMMEDIATE nf-immediate lp flags! ;
: NO-INTERPRET nf-compile-only lp flags! ;
: COMPILE-ONLY nf-compile-special lp flags! ;


: ' lp @ 1 parse-name lookup name>xt ;

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

ref: ->here		1 cells allot
ref: ->forth_wordlist	1 cells allot
ref: ->current		->forth_wordlist
ref: ->n_search_order	1 ,
ref: ->search_order	->forth_wordlist 7 cells allot
ref: ->literal_instr	handler: do_literal
ref: ->skip_instr	handler: do_skip
ref: ->fskip_instr	handler: do_fskip
ref: ->do_instr		handler: do_do
ref: ->plus_loop_instr	handler: do_plus_loop
ref: ->does_instr	handler: do_does
ref: ->state		0 ,
ref: ->base		10 ,
ref: ->to_in		0 ,
ref: ->source_id	0 ,
ref: ->source		2 cells allot
ref: ->tib		256 chars allot
ref: ->source_max_len	1 cells allot
ref: ->lineno		1 cells allot
ref: ->input		1 cells allot
