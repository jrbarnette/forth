\  Copyright 2023, by J. Richard Barnette. All Rights Reserved.

META-HOST-MODE definitions

variable offset          0 offset !
variable line-state      true line-state !

: .offset ( state -- ) ." /* " offset @ 4 .r ."  */" ;

: start-entry line-state @ if .offset then ;
: { start-entry ."  { " ;
: } ."  }," 1 offset +! false line-state ! ;
: }{ } { ;
: emit-nl cr true line-state ! ;

: target-create ( -- ) create offset @ , does> @ { .defs } ;

also META-DEFINERS definitions previous
: prim: target-create parse-name { .exec } emit-nl ;
