\  Copyright 2017, by J. Richard Barnette. All Rights Reserved.

: prim: { s" i_addname" .exec }{ parse-name .str }{ parse-name .exec } ;

hex
: IMMEDIATE    80 setflags ;
: NO-INTERPRET 40 setflags ;
: COMPILE-ONLY c0 setflags ;
decimal
