\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.

: escaped '\' emit ;
: escape? ( char quote -- )
       over = if escaped exit then
    dup '\' = if escaped exit then
    dup   7 = if drop escaped 'a' exit then
    dup   8 = if drop escaped 'b' exit then
    dup   9 = if drop escaped 't' exit then
    dup  10 = if drop escaped 'n' exit then
    dup  12 = if drop escaped 'f' exit then
    dup  13 = if drop escaped 'r' exit then
;
: octal-escape ( ctrl-char -- )
    base @ >r 8 base ! 0 <# # # # #> r> base ! escaped type ;
: escape ( char quote -- )
    escape? dup printable? if emit else octal-escape then ;
: escape-nul ( #nul -- ) ?dup if 0 do ." \0" loop ." 00" then ;
: escape-nul? ( #nul c -- #nul )
    ?dup if swap escape-nul [char] " escape 0 else 1+ then ;
: escape-string ( c-addr u -- )
    0 tuck ?do over i chars + c@ escape-nul? loop 2drop ;

: .c-char ( char -- ) [char] ' dup emit dup >r escape r> emit ;
: .c-string ( c-addr u -- ) '"' emit escape-string '"' emit ;
: .c-decimal ( u -- ) base @ swap decimal 1 .r base ! ;
: .c-hex ( u -- )
    base @ swap hex
    <# dup >r abs 0 #s 'x' hold '0' hold r> sign #> type
    base ! ;
: .c-cell ( c-addr u -- ) ." (cell_ft) (" type ." )" ;


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

: .{ ( -- ) ."   { " ;
: }, ( -- ) ."  }," ;

: .cell ( a-addr -- )  .{ ." .cell = " @ .c-hex }, ;
: .str ( a-addr -- )   .{ ." .str = " 1 cells .c-string }, ;
: .label ( a-addr -- )
    @ target-dict -
    .{ ." .label = &dictionary.dict_space[" .c-decimal ." ]" }, ;
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

: IMMEDIATE nf-immediate lp @ name-flags! ;
: COMPILE-ONLY nf-compile-only lp @ name-flags! ;
: COMPILE-SPECIAL nf-compile-special lp @ name-flags! ;


: ' lp @ 1 parse-name lookup name>xt ;

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

ref: ->here		1 cells allot
ref: ->forth_wordlist	1 cells allot
ref: ->current		->forth_wordlist
ref: ->n_search_order	1 ,
ref: ->search_order	->forth_wordlist 7 cells allot

ref: ->lineno		1 cells allot
ref: ->input		1 cells allot
