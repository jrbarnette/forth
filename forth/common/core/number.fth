\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  >NUMBER               6.1.0570 CORE                   31
\ ------  ------  ------  ------  ------  ------  ------  ------

: >DIGIT ( char -- u )
    [char] 0 - 9 over u< if
	17 - 25 over u< if 32 - 25 over u< if drop -1 exit then then 10 +
    then
;

: >NUMBER ( ud1 c-addr1 u1 -- ud2 c-addr2 u2 )
    begin dup while			( ud c-addr u )
	>r dup >r c@ >digit		( ud digit ) ( R: u c-addr )
	dup base @ u< invert if drop r> r> exit then
	>r base @ * >r base @ um* r> + r>
	rot dup >r + dup r> u< negate rot +
	r> char+ r> 1-
    repeat
;
