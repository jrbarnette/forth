\  Copyright 2023, by J. Richard Barnette. All Rights Reserved.

: C-CODE>
    begin refill while
	source tuck s" END-CODE" id= if >in ! exit then
	begin
	    [char] ` parse ?dup if type else drop then
	    [char] ` parse ?dup if evaluate else drop then
	>in @ over < 0= until drop cr
    repeat
;
