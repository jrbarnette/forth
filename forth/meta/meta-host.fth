\  Copyright 2023, by J. Richard Barnette. All Rights Reserved.

METADICT-HOST-MODE definitions

variable offset          0 offset !
variable line-state      true line-state !

: .offset ( state -- ) ." /* " offset @ 4 .r ."  */" ;

: start-entry line-state @ if .offset then ;
: { start-entry ."  { " ;
: } ."  }," 1 offset +! false line-state ! ;
: }{ } { ;
: emit-nl cr true line-state ! ;

:noname ." &meta_dictionary[" .c-decimal ." ]" ;
		entry: .meta-xt ip ( +n -- )
: target-create ( -- ) create offset @ , does> @ { .meta-xt } ;

also METADICT-DEFINERS definitions previous
: prim: target-create parse-name { .exec } emit-nl ;
