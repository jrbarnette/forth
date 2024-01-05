\  Copyright 2023, by J. Richard Barnette. All Rights Reserved.

only FORTH definitions
: include-file: ( "filename" -- ) parse-name included ;

HOST-MODE
TRANSCRIBE>
#include <stddef.h>

#include "forth.h"
#include "dictionary.h"


static xt_ft references[];

vminstr_ft meta_definitions[] = {
    FORTH>
    META-TARGET-MODE
    include-file: forth/c-gen/core/vmprim.fth
    include-file: forth/c-gen/core/control.fth

    HOST-MODE META-HOST-MODE definitions
    include-file: forth/meta/meta-target.fth
    include-file: forth/meta/meta-tokens.fth
    include-file: forth/common/internal/interpret.fth
    include-file: forth/meta/compile-file.fth

    META-HOST-MODE definitions
    compile-meta: forth/c-gen/core/stackprim.fth
    compile-meta: forth/c-gen/core/arithprim.fth
    compile-meta: forth/c-gen/core/memprim.fth
    compile-meta: forth/c-gen/core/multprim.fth
    compile-meta: forth/c-gen/exception/throwprim.fth

    META-HOST-MODE definitions also META-SPECIAL
    include-file: forth/common/internal/branches.fth

    META-DEFINITIONS
    include-file: forth/common/core/control.fth

    META-HOST-MODE definitions also META-SPECIAL
    include-file: forth/common/internal/doprolog.fth

    META-DEFINITIONS
    include-file: forth/common/core/docase.fth

    META-HOST-MODE
    compile-meta: forth/c-gen/core/memops.fth
    compile-meta: forth/c-gen/core/allot.fth
    compile-meta: forth/common/core/stackops.fth
    compile-meta: forth/common/core/arithops.fth
    compile-meta: forth/common/core/memops.fth

    compile-meta: forth/common/internal/ascii.fth
    compile-meta: forth/common/internal/namespace.fth
    compile-meta: forth/common/search/current.fth
    compile-meta: forth/common/internal/dictionary.fth
    compile-meta: forth/common/internal/wordlists.fth
    compile-meta: forth/meta/meta.fth

    TRANSCRIBE>
};
FORTH>
META-HOST-MODE definitions
: .target-ref
    ." (&meta_definitions[" >body @ .c-decimal ." ])" ;
TRANSCRIBE>

#define EXIT                ` ' exit             .target-ref `
#define DO_LITERAL          ` ' do-literal       .target-ref `
#define META_INITIALIZE     ` ' meta-initialize  .target-ref `
#define META_LINKNAME       ` ' link-name        .target-ref `
#define META_STARTNAME      ` ' meta-startname   .target-ref `
#define META_ADDNAME        ` ' meta-addname     .target-ref `
#define META_SETFLAGS       ` ' meta-setflags    .target-ref `
#define META_INTERPRET      ` ' meta-interpret   .target-ref `
#define META_COMPILE        ` ' meta-compile     .target-ref `
#define META_REFERENCE      ` ' meta-reference   .target-ref `


vminstr_ft initialize_forth[] = {
    FORTH>
    meta-init
    \ These initial files can contain only "prim:" definitions.
    \ Colon definitions can't work until we load vmprim because ";"
    \ depends on "EXIT".  Also, colon definitions are only as useful
    \ as the set of primitives already defined in the target.
    compile-target: forth/c-gen/core/stackprim.fth
    compile-target: forth/c-gen/core/arithprim.fth
    compile-target: forth/c-gen/core/memprim.fth
    compile-target: forth/c-gen/core/multprim.fth
    compile-target: forth/c-gen/core/vmprim.fth
    compile-target: forth/c-gen/exception/throwprim.fth

    \ From here forward we can make colon definitions, minding
    \ internal dependencies.

    \ Some constructs have implicit dependencies on LITERAL in the
    \ target dictionary, so get that done first.
    compile-target: forth/c-gen/core/literal.fth

    \ The "dictops" depend on "memops", and these are lower-level, so
    \ do them early.
    compile-target: forth/c-gen/core/memops.fth
    compile-target: forth/common/core/memops.fth
    compile-target: forth/c-gen/core/dictops.fth
    compile-target: forth/common/core/dictops.fth

    \ We want "arithops" early, but that depends on "control", which
    \ in turn depends on "stackops".
    compile-target: forth/common/core/stackops.fth
    compile-target: forth/c-gen/core/control.fth
    compile-target: forth/common/internal/mark.fth
    compile-target: forth/common/internal/branches.fth
    compile-target: forth/common/core/control.fth
    compile-target: forth/common/internal/markchain.fth
    compile-target: forth/common/internal/doprolog.fth
    compile-target: forth/common/core/docase.fth
    compile-target: forth/common/core/arithops.fth

    compile-target: forth/common/internal/ascii.fth
    compile-target: forth/common/internal/namespace.fth
    compile-target: forth/common/search/current.fth
    compile-target: forth/common/internal/dictionary.fth
    compile-target: forth/common/internal/wordlists.fth

    compile-target: forth/common/exception/catch.fth

    \ At this point we have working definitions for "dictionary",
    \ "stack", "arith", "mem", "mult", and "control" words.  We're
    \ ready to start making more complicated definitions.
    compile-target: forth/c-gen/core/terminal.fth
    compile-target: forth/common/core/terminal.fth
    compile-target: forth/c-gen/file/fileprim.fth
    compile-target: forth/c-gen/file/modes.fth
    compile-target: forth/common/core/source.fth
    compile-target: forth/common/core/parse.fth
    compile-target: forth/common/internal/error.fth

    compile-target: forth/c-gen/search/base.fth
    compile-target: forth/common/search/base.fth
    compile-target: forth/common/internal/lookup.fth

    compile-target: forth/common/core/number.fth

    compile-target: forth/common/core/state.fth
    compile-target: forth/common/internal/numparse.fth
    compile-target: forth/common/internal/tokens.fth
    compile-target: forth/common/internal/interpret.fth

    compile-target: forth/common/core/definers.fth

    compile-target: forth/common/core/comments.fth

    compile-target: forth/common/core/evaluate.fth

    also TARGET ['] EVALUATE previous
    meta-exit
    TRANSCRIBE>
};


char *init_forth_defs[] = {
    FORTH>
    include-source-text: forth/common/core/compile.fth
    include-source-text: forth/common/core/format.fth
    include-source-text: forth/common/core/quit.fth
    include-source-text: forth/common/file/include.fth
    include-source-text: forth/common/search/find.fth
    include-source-text: forth/common/search/vocab.fth
    include-source-text: forth/common/tools/tools.fth
    TRANSCRIBE>
    NULL,
};

#define NREFERENCE `nreference @ .c-decimal`
static xt_ft references[NREFERENCE] = { NULL };
FORTH>
