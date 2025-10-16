\  Copyright 2025, by J. Richard Barnette. All Rights Reserved.

variable offset
variable indent  0 indent !
: start-entries  0 offset ! ;
: next-entry 1 offset +! ;
: .indent indent @ spaces 0 indent ! ;

: { .indent ."  { " ;
: } ."  }," next-entry ;
: }{ } { ;

: .handler  ." .handler = " type ;
: .exec     ." .ip = &meta_dictionary[" .c-decimal ." ]" ;
: .cell     ." .cell = " .c-hex ;
: .expr     ." .cell = (cell_ft) (" type ." )" ;
: .id       ." .id = " .c-string ;

: .start ( state -- ) ." /* " offset @ 4 u.r ."  */" ;
: .end ( -- ) cr 0 indent ! ;

: //name ( np indent -- )  spaces ." // " name>string type ;
: .meta dup >r space type 12 r> - indent ! next-entry ;
