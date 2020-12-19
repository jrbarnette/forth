\  Copyright 2020, by J. Richard Barnette. All Rights Reserved.

: .field= ." ." count type ."  = " ;
: .content count chars + aligned >r ;
: entry: create parse-name counted, align 0 ] does> dup .field= .content ;

entry: .exec handler ( c-addr u -- ) type ;
entry: .str id ( c-addr u | 0 -- ) .c-string ;
entry: .str-expr id ( c-addr u -- ) type ;
entry: .expr cell ( c-addr u -- ) .c-cell ;
entry: .cell cell ( x -- ) .c-hex ;

entry: .offset offset ( +n -- ) .c-decimal ;
entry: .cdata cdata ( a-addr -- ) [ 1 cells ] literal .c-string ;
entry: .label ip ( +n -- ) ." &initialize_forth[" .c-decimal ." ]" ;
entry: .dict ref ( +n -- ) ." &dictionary.dict_space[" .c-decimal ." ]" ;
