\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.

host-mode

: graphic? ( char -- flag ) [char] ! 127 within ;
: .{ ( -- ) ."   { " ;
: }, ( -- ) ."  }," ;
: .cell ( a-addr -- )
    .{ ." .cell = " @ 1 .r },
;
: .str ( a-addr -- )
    .{ ." .str = " [char] " emit
    0 swap dup cell+ swap do			( #nul )
	i c@					( #nul char )
	dup 0<> if				( #nul !0 )
	    over if swap 0 do ." \0" loop ." 00" 0 swap then
	    dup graphic? if			( #nul graphic )
		dup [char] \ = over [char] " = or
		if [char] \ emit then emit
	    else				( #nul non-graphic )
		8 base !
		0 <# # # # [char] \ hold #> type
		decimal
	    then
	else					( #nul 0 )
	    drop 1+
	then
    1 chars +loop drop
    [char] " emit },
;
: .label ( a-addr -- )
    .{ ." .label = &dictionary[" @ 1 .r ." ]" },
;
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
\   tags-buffer		Tags for .entry, packed as bit fields
\   code-names-buffer	Storage for handler-names for .handler


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

: parse-word  ( char "<chars>ccc<char>" -- c-addr u )
    >r source chars + source drop >in @ chars +
    begin 2dup > while		( end c-addr ) ( R: char )
	dup c@ r@ bl = if 33 - 94 u< else r@ <> then
	if 2drop r> parse exit then
	1 >in +! char+
    repeat r> 2drop 0
;

: link, ( -- ) align lp @ label, ;
: link-name ( name -- ) lp ! ;

: handler: ( "<name>" -- )
    bl parse-word				( c-addr u )
    dup hnp @ dup handler, c!			( c-addr u )
    hnp @ char+ swap chars dup >r move		( ) ( R: nchars )
    r> char+ hnp +!				( )
;

: prim:
    here link, link-name
    bl parse-word str,
    handler:
;

here constant do-literal
handler: do_literal

: toupper ( char -- char )
    dup [char] a - 26 u< if [ char A char a - ] literal + then
;

: id= ( c-addr1 u1 c-addr2 u2 -- match? )
    rot over = if
	begin dup while			( c-addr1 c-addr2 u )
	    >r over c@ toupper over c@ toupper <> if
		r> drop 2drop false exit
	    then char+ swap char+ swap r> 1-
	repeat drop 2drop true
    else
	drop 2drop false
    then
;

hex

: flags! ( flags -- ) lp @ cell+ dup >r c@ or r> c! ;
: immediate 80 flags! ;
: no-interpret 40 flags! ;
: compile-only c0 flags! ;

: >flags ( name -- xt ) cell+ c@ c0 and ;
: >id ( name -- c-addr u ) cell+ count 1f and ;
: >xt ( name -- xt ) cell+ dup c@ 1f and 1+ chars + aligned ;

: target-lookup ( c-addr u -- name )
    lp @ begin
	>r 2dup r@ >id id= if		( id id tid ) ( R: name )
	    2drop r> true
	else
	    r> @ dup if
		data-buffer + false
	    else
		drop 2drop 0 true
	    then
	then				( c-addr u name flag )
    until
;

: compile, label, ;

decimal

variable state	0 state !
