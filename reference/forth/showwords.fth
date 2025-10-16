only FORTH definitions

: is-vocab? ( nt -- flag )
    name>xt cell+ @ [ ' METACOMPILER cell+ @ ] literal = ;
: .indent ( n -- ) 4 * spaces ;
: .name ( nt -- )
    dup 9 u.r space dup name>xt+flags
	dup immediate? if 'I' else bl then emit
	compile-only? if 'C' else bl then emit
	drop space
    name>string type ;

: next-name
    begin                              ( nt*l-1 l nt )
	@ dup if exit then             ( nt*l-1 l 0 )
	drop 1- dup 0= if exit then    ( nt*l-1 l-1 )
	swap                           ( nt*l-2 l-1 nt )
    again ;


: show-dict ( wid -- )
    base @ >r hex
    1 swap
    begin next-name dup while
	over .indent dup .name cr
	dup is-vocab? if swap 1+ over name>xt >body then
    repeat drop r> base ! ;

.( FORTH-WORDLIST ) cr
forth-wordlist show-dict
