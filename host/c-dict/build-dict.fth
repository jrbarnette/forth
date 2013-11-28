\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.

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

256 constant #defn-buffer
create defn-buffer #defn-buffer cells allot
variable dp defn-buffer dp !

2				constant #tag-bits
1 #tag-bits lshift 1-		constant tag-mask
1 cells 8 * #tag-bits /		constant #tags/cell
create tag-buffer
    #defn-buffer #tags/cell 1- + #tags/cell /
    cells allot
: tag-idx ( idx -- shift a-addr )
    #tags/cell /mod >r #tag-bits * r> cells tag-buffer +
;
: tag@ ( idx -- tag ) tag-idx @ swap rshift tag-mask and ;
: tag! ( tag idx -- ) tag-idx >r lshift r@ @ or r> ! ;
: clear-defn #defn-buffer 0 do 0 defn-buffer i cells + ! loop ;
: clear-tags #defn-buffer 0 do i tag-idx ! #tags/cell +loop ;

: here dp @ ;
\ must be careful now; we have two versions of HERE

: dp>tag defn-buffer - 1 cells / ;
: mark-string  1 here dp>tag tag! ;
: mark-label   2 here dp>tag tag! ;
: mark-handler 3 here dp>tag tag! ;
: mark-string? here dup aligned <> if mark-string then ;
: align ( -- ) mark-string? here aligned dp ! ;
: allot ( n -- )
    here + dup aligned here aligned <> if mark-string? then dp !
;
: , ( x -- ) here 1 cells allot ! ;
: c, ( char -- ) here 1 chars allot c! ;
: str, ( c-addr u -- )
    here aligned >r chars dup >r here swap move r> allot
    here aligned r> begin 2dup > while
	1 over dp>tag tag!
    cell+ repeat
;

: flush-definition
    align here defn-buffer begin 2dup > while
	dup dup dp>tag tag@ .entry
    cell+ repeat defn-buffer dp !
;

5 ,
bl parse A-VERY-LONG-NAME str,
flush-definition
