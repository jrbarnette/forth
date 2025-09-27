: LITERAL do-literal do-literal compile, , ; compile-special
: LITERAL ['] do-literal compile, , ; compile-special

: parse-valid-name ( "name" -- nt )
    parse-name lookup dup 0= if -13 .error then ;
: CHAR ( "name" -- char )  parse-name 0= if -16 .error then c@ ;

: ' ( "name" -- xt )  parse-valid-name name>xt ;

: [COMPILE] ' compile, ; compile-special

: ['] ' [compile] literal ; compile-special
: [CHAR] char [compile] literal ; compile-special

: POSTPONE
    parse-valid-name name>xt+flags immediate? 0=
    if [compile] literal ['] compile, then compile, ; compile-special

: S" [char] " parse postpone do-s" dup , chars, align ; compile-special
: C" [char] " parse postpone do-c" counted, align ; compile-special
