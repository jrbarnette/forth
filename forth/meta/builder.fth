\  Copyright 2023, by J. Richard Barnette. All Rights Reserved.

HOST-MODE definitions

:noname ." &builder_defs[" .c-decimal ." ]" ;
entry: .defs ip ( +n -- )

variable offset          0 offset !
variable emit-state      0 emit-state !
variable line-state      true line-state !

: .indent ( state -- ) if 4 else 0 then spaces ;
: .offset ( state -- ) ." /* " offset @ 4 .r ."  */" .indent ;

: start-entry line-state @ if emit-state @ .offset then ;
: { start-entry ."  { " ;
: } ."  }," 1 offset +! false line-state ! ;
: }{ } { ;
: emit-nl cr true line-state ! ;

: /*name*/ ( name -- )
    ." /* " name>string dup >r type 15 r> - spaces ."  */ " ;

: create-builder-def ( -- )
    create current-name , offset @ , does> 2@ ( offset name )
	{ ( /*name*/ ) drop .defs }
;

\ : create-builder-def ( -- )
\     ['] create-builder-def builder-defs-wordlist do-in-wordlist ;

HOST-MODE also BUILDER-INTERP definitions previous
: prim: create-builder-def parse-name { .exec } emit-nl ;

BUILDER-INTERP-MODE
