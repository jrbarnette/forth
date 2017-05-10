\  Copyright 2017, by J. Richard Barnette. All Rights Reserved.

wordlist constant META-INTERPRET
wordlist constant META-COMPILE

variable META-MODE  0 meta-mode !

: EVAL-NUMBER ( c-addr len -- n -1 | 0 )
    over c@ [char] '-' = >r	( R: neg? )
    r@ + swap r@ chars - swap
    0 0 2swap >number		( ulo uhi str len )
    if 2drop r> 2drop 0 else 2drop r> if negate then -1 then
;

: DIRECT { s" NULL" .cell } ;

: META[
    { s" meta_interpret" .exec } meta-interpret meta-mode !
    begin
	begin parse-name dup while
	    2dup meta-mode @ search-wordlist if
		>r 2drop r> execute
		meta-mode @ 0= if exit then
	    else
		2dup eval-number if
		    direct { s" meta_literal" .exec }{ c-hex .cell } 2drop
		else
		    { .str }
		then
	    then
	repeat 2drop
    refill 0= until
;

\ : BASE     BASE ;
\ : DECIMAL  DECIMAL ;
\ : HEX      HEX ;
\ : (        postpone ( ;
\ : .(       postpone .( ;
\ : \        postpone \ ;

meta-interpret set-current
: ]META direct 0 meta-mode ! ;

also direct-assembler definitions previous
