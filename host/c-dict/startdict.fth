\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.

vocabulary TARGET
also target definitions

: .{ ( -- ) ."   { " ;
: }, ( -- ) ."  }," ;

: escape-nul ( #nul -- ) ?dup if 0 do ." \0" loop ." 00" then ;
: escape-graphic ( c -- )
    dup [char] \ = over [char] " = or if [char] \ emit then emit ;
: escape-non-graphic ( c -- )
    base @ swap 8 base ! 0 <# # # # [char] \ hold #> type base ! ;
: escape-char ( c -- )
    dup graphic? if escape-graphic else escape-non-graphic then ;
: escape ( #nul c -- #nul )
    ?dup if swap escape-nul escape-char 0 else 1+ then ;
: escape-string ( c-addr u -- )
    over >r chars + 0 swap r> do i c@ escape 1 chars +loop drop ;
: c-string ( c-addr u -- ) [char] " emit escape-string [char] " emit ;
: c-decimal ( u -- ) base @ swap decimal 1 .r base ! ;
: c-hex ( u -- c-addr u ) base @ swap hex ." 0x" 1 .r base ! ;

: .cell ( a-addr -- )  .{ ." .cell = " @ c-hex }, ;
: .str ( a-addr -- )   .{ ." .str = " 1 cells c-string }, ;
: .label ( a-addr -- ) .{ ." .label = &dictionary[" @ c-decimal ." ]" }, ;
: .handler ( a-addr -- ) .{ ." .handler = " @ count type }, ;

here
    ' .cell ,
    ' .str ,
    ' .label ,
    ' .handler ,
: .entry ( a-addr tag -- ) cells [ swap ] literal + @ execute ;


\ We accumulate definitions into a local buffer that we flush at the
\ end of processing.
\
\ The buffer is needed for cases where we backpatch content, as for
\ forward branches (e.g. IF, WHILE) and IMMEDIATE.
\
\ The buffer has the following parts:
\   data-buffer		The actual cells of the definition
\   tag-buffer		Tags for .entry, packed as bit fields
\   hdlrs-buffer	Storage for handler-names for .handler


24 1024 *			constant  #data-buffer
#data-buffer cells		create    data-buffer  allot

2				constant  #tag-bits
1 #tag-bits lshift 1-		constant  tag-mask
1 cells 8 * #tag-bits /		constant  #tags/cell

#data-buffer #tags/cell + 1-
#tags/cell / cells		create   tag-buffer allot
1024 chars			create   hdlrs-buffer allot

variable  dp	data-buffer dp !
variable  hnp	hdlrs-buffer hnp !
variable  lp	0 lp !

: tag-addr ( a-addr -- shift a-addr )
    data-buffer - 1 cells /
    #tags/cell /mod >r #tag-bits * r> cells tag-buffer +
;
: tag@ ( a-addr -- tag ) tag-addr @ swap rshift tag-mask and ;
: tag! ( tag a-addr -- ) tag-addr >r lshift r@ @ or r> ! ;


: here dp @ ;
\ must be careful now; we have two versions of HERE

: mark-string  1 here 1 cells - tag! ;
: mark-label   2 here 1 cells - tag! ;
: mark-handler 3 here 1 cells - tag! ;
: mark-string? here dup aligned <> if 1 here tag! then ;

\ target dictionary operations
\   HERE ALIGN ALLOT , C,
\   str, handler, label,
\   prim: handler:

: align ( -- ) mark-string? here aligned dp ! ;
: allot ( n -- )
    here + dup aligned here aligned <> if mark-string? then dp !
;
: , ( x -- ) here 1 cells allot ! ;
: c, ( char -- ) here 1 chars allot c! ;
: str, ( c-addr u -- )
    align here >r dup c, here swap chars dup allot move
    here aligned r> aligned
    begin 2dup u> while 1 over tag! cell+ repeat 2drop
;
: handler, ( c-addr -- ) align , mark-handler ;
: label, ( addr -- ) dup if data-buffer - , mark-label else , then ;

: flush-target-dictionary
    decimal
    align here data-buffer begin 2dup > while
	dup data-buffer - ." /* " 5 .r ."  */"
	dup dup tag@ .entry cr
    cell+ repeat 2drop
;

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

base @ hex
: flags! ( flags -- ) lp @ cell+ dup >r c@ or r> c! ;
: immediate 80 flags! ;
: no-interpret 40 flags! ;
: compile-only c0 flags! ;
base !
