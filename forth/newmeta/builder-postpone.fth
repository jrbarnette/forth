\  Copyright 2025, by J. Richard Barnette. All Rights Reserved.

BUILDER-HOST-MODE

: literal-reference  0 ref-mode ! execute builder-literal ;

also BUILDER-SHADOW definitions previous
: VARIABLE
    s" do_variable" addname
    s" CELL_SIZE" literal-expr
    1 ref-mode ! [BUILDER] ALLOT ;
: CONSTANT
    s" do_constant" addname
    1 ref-mode ! [BUILDER] , ;

: POSTPONE
    parse-valid-name name>xt+flags immediate? 0= if
	literal-reference [ ' COMPILE, ] literal
    then 2 ref-mode ! execute ; compile-special
: [COMPILE] ' 2 ref-mode ! execute ; compile-special
: ['] ' literal-reference ; compile-special
