\  Copyright 2025, by J. Richard Barnette. All Rights Reserved.

HOST-MODE
\ Shared with BUILDER-HOST-MODE...
: unlink-name  current-name dup name>prev link-name ;

METADICT-HOST-MODE

: prim-create  create offset @ , does> { @ .exec } ;

: target-create  prim-create current-name 15 //name cr ;

: start-meta-colon
    target-create unlink-name .start { s" do_colon" .handler } .end ;
: end-meta-colon dup link-name name>xt >body cell+ ;

also METADICT-DEFINERS definitions previous
: prim:  prim-create .start { parse-name .handler } .end ;
: VARIABLE
    target-create .start { s" do_variable" .handler }{ 0 .cell } .end ;
: CONSTANT
    target-create .start { s" do_constant" .handler }{ .cell } .end ;
