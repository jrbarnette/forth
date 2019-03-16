: .offset ( n -- ) ." /* " offset @ 3 .r ."  */" spaces ;
: { ." { " ;

: {
    meta-state @ emit-state @ <> if
	emit-state @ if 5 .offset { ." .id = NULL" } then
	meta-state @ dup emit-state !
	?dup if
	    1 .offset
	    { 1- if s" meta_compile" else s" meta_interpret" then .exec }
	then
    then meta-state @ if 5 else 1 then .offset {
;
