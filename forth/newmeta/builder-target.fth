\  Copyright 2025, by J. Richard Barnette. All Rights Reserved.

BUILDER-HOST-MODE

: parse-valid-name
    get-order only BUILDER-TARGET parse-valid-name >r set-order r> ;

: '  parse-valid-name name>xt ;
: [BUILDER]  ' compile, ; compile-special

: builder-literal
    state @ if
	2 ref-mode ! [BUILDER] DO-LITERAL
	s" COMMA" .start-meta .end
    then ;


also BUILDER-TARGET definitions previous
: <C> ';' parse literal-expr builder-literal ;


also BUILDER-SHADOW definitions previous
: \ postpone \ ; immediate
: ( postpone ( ; immediate
: .( postpone .( cr ; immediate

: [ postpone [ ; immediate
: ] ] ;

: IMMEDIATE        immediate       nf-immediate       build-setflags ;
: COMPILE-ONLY     compile-only    nf-compile-only    build-setflags ;
: COMPILE-SPECIAL  compile-special nf-compile-special build-setflags ;

: :  s" do_colon" startname ] ;
: ;  2 ref-mode ! [BUILDER] EXIT linkname postpone [ ; compile-special

: handler: parse-name literal-handler builder-literal ; immediate

: [CHAR] char literal-cell builder-literal ; compile-special
