\ Copyright 2015, by J. Richard Barnette, All Rights Reserved.

hex
: show-dict
    name-ptr begin @ dup while
	dup . dup cell+ count 2dup
	1f and chars + aligned @ .
	dup e0 and 2 .r space 1f and type cr
    repeat drop
;
show-dict
