\  Copyright 2023, by J. Richard Barnette. All Rights Reserved.

HOST-MODE definitions
: dump-wordlist
    @ begin dup while
       ." // " dup name>string type cr name>prev
    repeat drop
;


only FORTH definitions
: include-file: ( "filename" -- ) parse-name included ;

HOST-MODE
TRANSCRIBE>
#include <stddef.h>

#include "forth.h"
#include "dictionary.h"
#include "meta.h"


vminstr_ft builder_defs[] = {
    FORTH>
    builder-interp-mode
    include-file: forth/c-gen/core/vmprim.fth
    include-file: forth/c-gen/core/control.fth

    include-file: forth/meta/build-defs.fth
    include-file: forth/meta/tokens.fth
    include-file: forth/common/internal/interpret.fth
    include-file: forth/meta/compile-file.fth

    compile-file: forth/c-gen/core/stackprim.fth
    compile-file: forth/c-gen/core/arithprim.fth
    compile-file: forth/c-gen/core/memprim.fth
    compile-file: forth/c-gen/core/multprim.fth
    compile-file: forth/c-gen/exception/throwprim.fth

    compile-file: forth/c-gen/core/memops.fth
    compile-file: forth/common/core/memops.fth
    compile-file: forth/c-gen/core/allot.fth
    TRANSCRIBE>
};
FORTH>
