\  Copyright 2023, by J. Richard Barnette. All Rights Reserved.

METADICT-HOST-MODE

variable offset          0 offset !

: .start ( -- ) ." /* " offset @ 4 .r ."  */" ;
: .end   ( -- ) cr ;

: { ."  { " ;
: } ."  }," 1 offset +! ;
: }{ } { ;

:noname ." &meta_dictionary[" .c-decimal ." ]" ;
		entry: .meta-xt ip ( +n -- )
: target-create ( -- ) create offset @ , does> @ { .meta-xt } ;

also METADICT-DEFINERS definitions previous
: prim: target-create parse-name .start { .exec } .end ;
