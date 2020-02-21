\  Copyright 2020, by J. Richard Barnette. All Rights Reserved.

: .exec ( c-addr u -- ) ." .handler = " type ;
: .cdata ( a-addr -- ) 1 cells ." .cdata = " .c-string ;
: .str ( c-addr u -- ) ." .id = " .c-string ;
: .value ( xt -- ) ." .cell = " execute ;
: .expr ( c-addr u -- ) ['] .c-cell .value ;
: .cell ( u -- ) ['] .c-hex .value ;
: .dict ( u -- ) ." .ptr = &dictionary.dict_space[" .c-decimal ." ]" ;

variable offset      0 offset !

: { ." { " ;
: } ."  }," 1 offset +! ;
: }{ } space { ;
