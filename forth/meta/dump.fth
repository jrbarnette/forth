HOST-MODE definitions
create flag-chars
    '-' C, '-' C, \ 00 default semantics
    'C' C, 'O' C, \ 01 compile-only
    'I' C, 'M' C, \ 10 IMMEDIATE
    'S' C, 'P' C, \ 11 special

: .flags
    6 rshift 1 lshift flag-chars + 2 type
;

: dump-wordlist
    @ begin dup while
     ." //   "
     dup name>xt+flags .flags space drop dup
     name>string type cr name>prev
    repeat drop ;

: show-vocabularies
    ." // META-TARGET" cr
     meta-target-wordlist dump-wordlist cr
    ." // META-DEFINERS" cr
     ['] META-DEFINERS >body dump-wordlist cr
    ." // META-SPECIAL" cr
     ['] META-SPECIAL >body dump-wordlist cr
    ." // HOST" cr
     ['] HOST >body dump-wordlist
;
cr show-vocabularies

: show-vocabularies
    ." // TARGET" cr
     ['] TARGET >body dump-wordlist cr
    ." // HOST" cr
     ['] HOST >body dump-wordlist
;
cr show-vocabularies
