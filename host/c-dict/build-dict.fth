\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.

host-mode

: graphic? ( char -- flag ) [char] ! 127 within ;
: .{ ( -- ) ."     { " ;
: }, ( -- ) ."  }," cr ;
: .cell ( a-addr -- )
    base @ >r decimal .{ ." .cell = " @ 1 .r }, r> base !
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
		base @ >r 8 base !
		0 <# # # # [char] \ hold #> type
		r> base !
	    then
	else					( #nul 0 )
	    drop 1+
	then
    1 chars +loop drop
    [char] " emit },
;
: .label ( a-addr -- )
    base @ >r decimal .{ ." .label = &dictionary[" @ 1 .r ." ]" }, r> base !
;
: .handler ( a-addr -- ) .{ ." .handler = " @ count type }, ;

here
    ' .cell , 
    ' .str , 
    ' .label , 
    ' .handler , 
: .entry ( a-addr tag -- ) cells [ swap ] literal + @ execute ;


\ We accumulate definitions into a local buffer that we flush at the
\ start of each new definition, and at the end.
\
\ The buffer is needed for cases where we backpatch content, most
\ notably for forward branches (e.g. IF, WHILE).
\
\ The buffer has the following parts:
\   defn-buffer		The actual cells of the definition
\   tags-buffer		Tags for .entry, packed as bit fields
\   names-buffer	Storage for handler-names for .handler

256				constant #defn-buffer

create		defn-buffer	#defn-buffer cells allot
variable	dp		defn-buffer dp !

2				constant #tag-bits
1 #tag-bits lshift 1-		constant tag-mask
1 cells 8 * #tag-bits /		constant #tags/cell

create		tag-buffer
    #defn-buffer #tags/cell 1- + #tags/cell /
    cells allot
: tag-addr ( a-addr -- shift a-addr )
    defn-buffer - 1 cells /
    #tags/cell /mod >r #tag-bits * r> cells tag-buffer +
;
: tag@ ( a-addr -- tag ) tag-addr @ swap rshift tag-mask and ;
: tag! ( tag a-addr -- ) tag-addr >r lshift r@ @ or r> ! ;

create		names-buffer		256 chars allot
variable	np			names-buffer np !

: here dp @ ;
\ must be careful now; we have two versions of HERE

: mark-string  1 here 1 cells - tag! ;
: mark-label   2 here 1 cells - tag! ;
: mark-handler 3 here 1 cells - tag! ;
: mark-string? here dup aligned <> if 1 here tag! then ;
: align ( -- ) mark-string? here aligned dp ! ;
: allot ( n -- )
    here + dup aligned here aligned <> if mark-string? then dp !
;
: , ( x -- ) here 1 cells allot ! ;
: c, ( char -- ) here 1 chars allot c! ;
: str, ( c-addr u -- )
    here dup >r swap chars dup allot move
    here aligned r> aligned
    begin 2dup u> while 1 over tag! cell+ repeat
;
: handler, ( c-addr u -- )
    np @ , mark-handler
    dup >r dup np @ c! np @ char+ swap chars move r> 1+ np +!
;

: flush-definition
    align here defn-buffer begin 2dup > while
	dup dup tag@ .entry
    cell+ repeat
    defn-buffer dp ! names-buffer np !
    #defn-buffer 0 do 0 defn-buffer i cells + ! loop
    #defn-buffer 0 do defn-buffer i cells + tag-addr ! #tags/cell +loop
;

1 ,
bl parse NAME str,
flush-definition

2 ,
bl parse AN-EXTRA-LONG-NAME str,
flush-definition

3 ,
bl parse x_plus handler,
flush-definition
