\  Copyright 2023, by J. Richard Barnette. All Rights Reserved.

only FORTH definitions
: include-file: ( "filename" -- ) parse-name included ;

HOST-MODE
TRANSCRIBE>
#include <stddef.h>

#include "forth.h"
#include "dictionary.h"
#include "meta.h"


vminstr_ft meta_definitions[] = {
    FORTH>
    META-TARGET-MODE
    include-file: forth/c-gen/core/vmprim.fth
    include-file: forth/c-gen/core/control.fth

    HOST-MODE
    include-file: forth/meta/meta-target.fth
    include-file: forth/meta/tokens.fth
    include-file: forth/common/internal/interpret.fth
    include-file: forth/meta/compile-file.fth

    compile-file: forth/c-gen/core/stackprim.fth
    compile-file: forth/c-gen/core/arithprim.fth
    compile-file: forth/c-gen/core/memprim.fth
    compile-file: forth/c-gen/core/multprim.fth
    compile-file: forth/c-gen/exception/throwprim.fth

    META-HOST-MODE definitions also META-SPECIAL
    include-file: forth/common/internal/branches.fth

    META-DEFINITIONS
    include-file: forth/common/core/control.fth

    META-HOST-MODE definitions also META-SPECIAL
    include-file: forth/common/internal/doprolog.fth

    META-DEFINITIONS
    include-file: forth/common/core/docase.fth

    META-HOST-MODE
    compile-file: forth/c-gen/core/memops.fth
    compile-file: forth/c-gen/core/allot.fth
    compile-file: forth/common/core/stackops.fth
    compile-file: forth/common/core/arithops.fth
    compile-file: forth/common/core/memops.fth

    compile-file: forth/common/internal/ascii.fth
    compile-file: forth/common/internal/namespace.fth
    compile-file: forth/common/search/current.fth
    compile-file: forth/common/internal/dictionary.fth
    compile-file: forth/common/internal/wordlists.fth
    compile-file: forth/meta/meta.fth

    TRANSCRIBE>
};
FORTH>
META-HOST-MODE definitions
: .target-ref
    ." (&meta_definitions[" >body @ .c-decimal ." ])" ;
TRANSCRIBE>

#define DO_LITERAL          ` ' do-literal       .target-ref `
#define META_INITIALIZE     ` ' meta-initialize  .target-ref `
#define META_LINKNAME       ` ' link-name        .target-ref `
#define META_STARTNAME      ` ' meta-startname   .target-ref `
#define META_ADDNAME        ` ' meta-addname     .target-ref `
#define META_SETFLAGS       ` ' meta-setflags    .target-ref `
#define META_INTERPRET      ` ' meta-interpret   .target-ref `
#define META_COMPILE        ` ' meta-compile     .target-ref `
#define META_REFERENCE      ` ' meta-reference   .target-ref `

FORTH>
\ META-HOST-MODE definitions
\ create flag-chars
\     '-' C, '-' C, \ 00 default semantics
\     'C' C, 'O' C, \ 01 compile-only
\     'I' C, 'M' C, \ 10 IMMEDIATE
\     'S' C, 'P' C, \ 11 special
\ 
\ : .flags
\     6 rshift 1 lshift flag-chars + 2 type
\ ;
\ 
\ : dump-wordlist
\     @ begin dup while
\ 	." //   "
\ 	dup name>xt+flags .flags space drop dup
\ 	name>string type cr name>prev
\     repeat drop ;
\ 
\ : show-vocabularies
\     ." // META-TARGET" cr
\ 	meta-target-wordlist dump-wordlist cr
\     ." // META-DEFINERS" cr
\ 	['] META-DEFINERS >body dump-wordlist cr
\     ." // META-SPECIAL" cr
\ 	['] META-SPECIAL >body dump-wordlist cr
\     ." // HOST" cr
\ 	['] HOST >body dump-wordlist
\ ;
\ cr show-vocabularies
