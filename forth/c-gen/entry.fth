\  Copyright 2020, by J. Richard Barnette. All Rights Reserved.

: .field= ." ." count type ."  = " ;
: .content count chars + aligned >r ;
: entry: create parse-name counted, align 0 ] does> dup .field= .content ;

entry: .exec handler ( c-addr u -- ) type ;
entry: .str id ( c-addr u | 0 -- ) ?dup if .c-string else ." NULL" then ;
entry: .expr cell ( c-addr u -- ) .c-cell ;
entry: .cell cell ( u -- ) .c-hex ;

entry: .offset offset ( a-addr -- ) .c-decimal ;
entry: .cdata cdata ( a-addr -- ) [ 1 cells ] literal .c-string ;
entry: .dict ref ( u -- ) ." &dictionary.dict_space[" .c-decimal ." ]" ;
entry: .label ip ( u -- ) ." &initialize_forth[" .c-decimal ." ]" ;
