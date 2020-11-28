\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.

\ We accumulate the target dictionary into a local buffer that we
\ flush at the end of processing.
\
\ The buffer is needed for cases where we backpatch content, as for
\ forward branches (e.g. IF, WHILE) and IMMEDIATE.
\
\ The buffer has the following parts:
\   target-dict		The actual cells of the target dictionary
\   tag-buffer		Tags for .entry, packed as bit fields
\   strings-buffer	Storage for strings for .handler and .expr

24 1024 *			constant  #target-dict
#target-dict cells		create    target-dict  allot

3				constant  #tag-bits
1 #tag-bits lshift 1-		constant  tag-mask
1 cells 8 * #tag-bits /		constant  #tags/cell

#target-dict #tags/cell + 1-
#tags/cell / cells		create   tag-buffer allot
4096 chars			create   strings-buffer allot

variable  strp	strings-buffer strp !

: tag-addr ( a-addr -- shift a-addr )
    target-dict - 1 cells /
    #tags/cell /mod >r #tag-bits * r> cells tag-buffer +
;
: tag@ ( a-addr -- tag ) tag-addr @ swap rshift tag-mask and ;
: tag! ( tag a-addr -- ) tag-addr >r lshift r@ @ or r> ! ;

: .{ ( -- ) ." { " ;
: }, ( -- ) ."  }," ;

: .counted ( a-addr -- )  @ count type ;
: .cell-number ( a-addr -- )  @ .c-hex ;
: .cell-expr ( a-addr -- )  ." (cell_ft) (" .counted ." )" ;

: .value ( xt -- )  .{ ." .cell = " execute }, ;

: .cell ( a-addr -- )  ['] .cell-number .value ;
: .str ( a-addr -- )   .{ ." .str = " 1 cells .c-string }, ;
: .label ( a-addr -- )
    @ target-dict -
    .{ ." .label = &dictionary.dict_space[" .c-decimal ." ]" }, ;
: .handler ( a-addr -- ) .{ ." .handler = " .counted }, ;
: .expr ( a-addr -- )  ['] .cell-expr .value ;

: .invalid ." invalid tag at offset " target-dict - 1 u.r cr ;

\ .entry is a defintion that will print the value of a target
\ dictionary cell, given a pointer to the cell in the buffer and the
\ tag.

here
    ' .cell ,		\ tag #0
    ' .str ,		\ tag #1
    ' .label ,		\ tag #2
    ' .handler ,	\ tag #3
    ' .expr ,		\ tag #4
    ' .invalid ,	\ tag #5
    ' .invalid ,	\ tag #6
    ' .invalid ,	\ tag #7
: .entry ( a-addr tag -- ) cells [ swap ] literal + @ execute ;


vocabulary HOST
vocabulary TARGET

only FORTH also HOST definitions

\ target dictionary operations
\   HERE ALIGN ALLOT , C,
\   str, handler, label,
\   prim: handler: ref:

: HERE target-dict @ ;
\ must be careful now; we have two versions of HERE

: mark-string  here swap begin 2dup u> while 1 over tag! cell+ repeat 2drop ;
: mark-string? here dup aligned <> if 1 here tag! then ;
: mark-label   2 here 1 cells - tag! ;
: mark-handler 3 here 1 cells - tag! ;
: mark-expr    4 here 1 cells - tag! ;


: ALIGN ( -- ) mark-string? here aligned target-dict ! ;
: ALLOT ( n -- )
    here + dup aligned here aligned <> if mark-string? then target-dict !
;
: , ( x -- ) here 1 cells allot ! ;
: C, ( char -- ) here 1 chars allot c! ;

\ Non-standard allocation definitions; used to ensure cells are
\ properly tagged.

\ str, - allot storage for a string.
\ handler, - allot storage for a pointer to a handler function.
\ label, - allot storage for a dictionary address reference.
\ expr, - allot storage for a cell value calculated with a C expression.

: str, ( c-addr u -- )
    align here >r dup c, here swap chars dup allot move r> mark-string
;
: handler, ( c-addr -- ) align , mark-handler ;
: label, ( addr -- ) align dup , if mark-label then ;
: expr, ( c-addr -- ) align , mark-expr ;


: link@ ( -- ) target-dict cell+ @ ;
: link, ( -- ) align link@ label, ;
: link-name ( name -- ) target-dict cell+ ! ;

: >strings ( c-addr u -- c-addr' )
    strp @ 2dup c! char+ swap chars dup >r move strp @ r> char+ strp +! ;

: handler: ( "<name>" -- )  parse-name >strings handler, ;
: <C> ( "<c-expr>" -- )  ';' parse >strings expr, ;

: prim: here link, link-name parse-name str, handler: ;
: compile, label, ;

: ref: create here [ only forth ] , immediate does> @ [ also host ] label, ;

: IMMEDIATE nf-immediate link@ name-flags! ;
: COMPILE-ONLY nf-compile-only link@ name-flags! ;
: COMPILE-SPECIAL nf-compile-special link@ name-flags! ;


: ' parse-name link@ wid-lookup ?dup if name>xt else -13 throw then ;

: flush-target-dictionary
    decimal
    align here target-dict begin 2dup > while
	dup target-dict - ." /* " 5 u.r ."  */  "
	dup dup tag@ .entry cr
    cell+ repeat 2drop
;

\     struct {
\ 	addr_ft		here;		    /* HERE */
\ 	name_p		forth_wordlist;	    /* FORTH-WORDLIST */
\
\ 	size_t		lineno;
\ 	FILE *		input;
\     } dict_static_data;

ref: ->here		target-dict dup cell+ swap ! mark-label
ref: ->forth_wordlist	0 , mark-label

ref: ->lineno		1 cells allot
ref: ->input		1 cells allot
