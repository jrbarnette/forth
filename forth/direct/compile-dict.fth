\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

only FORTH definitions

: OPEN-SOURCE-FILE ( "filename" -- fileid )
    parse-name 12 spaces ." /* ==== " 2dup type ."  ==== */" cr
    r/o open-file abort" failed to open file" ;
: INTERPRET-META-FILE
    \ XXX TARGET overrides [ and ], so this construct fails:
    \   target [ also target ] <direct> [ previous ]
    only target meta-state @ 0= if also direct then
    begin refill if ['] interpret catch ?dup else 0 -1 then until
    only forth source-id close-file drop throw ;
: COMPILE-FILE: ( "filename" -- )
    open-source-file ['] interpret-meta-file with-input-source ;

create SOURCE-LINE 256 dup chars allot constant LINE-SIZE
: INCLUDE-SOURCE-TEXT: ( "filename" -- )
    open-source-file
    begin dup >r source-line dup line-size r> read-line drop while
        4 spaces .c-string ',' emit cr
    repeat 2drop close-file drop ;


>>> #include <stddef.h>
>>>
>>> #include "forth.h"
>>> #include "dictionary.h"
>>> #include "direct.h"
>>>
>>>
>>> DIRECT_FORTH(initialize_forth) // {
0 meta-state !
    compile-file: forth/direct/startdict.fth
    \ Repeat our early primitive definitions, this time to generate
    \ initial target dictionary content.
    \
    \ These initial files can contain only "prim:" definitions.
    \ Colon definitions can't work until we load vmprim because ";"
    \ depends on "EXIT".  Also, colon definitions are only as useful
    \ as the set of primitives we have available.
    compile-file: forth/c-gen/core/stackprim.fth
    compile-file: forth/c-gen/core/arithprim.fth
    compile-file: forth/c-gen/core/memprim.fth
    compile-file: forth/c-gen/core/multprim.fth
    compile-file: forth/c-gen/core/vmprim.fth
    compile-file: forth/c-gen/exception/throwprim.fth

    \ From here forward we can make colon definitions, minding
    \ internal dependencies.

    \ Some constructs have implicit dependencies on LITERAL in the
    \ target dictionary, so get that done first.
    compile-file: forth/c-gen/core/literal.fth

    \ The "dictops" depend on "memops", and these are lower-level, so
    \ do them early.
    compile-file: forth/c-gen/core/memops.fth
    compile-file: forth/common/core/memops.fth
    compile-file: forth/c-gen/core/dictops.fth

    \ We want "arithops" early, but that depends on "control", which
    \ then depends on "stackops".
    compile-file: forth/common/core/stackops.fth
    compile-file: forth/c-gen/core/control.fth
    compile-file: forth/common/core/control.fth
    compile-file: forth/common/core/arithops.fth
    compile-file: forth/common/internal/dictionary.fth
    compile-file: forth/common/exception/catch.fth

    \ At this point we have working definitions for "dictionary",
    \ "stack", "arith", "mem", "mult", and "control" words.  We're
    \ ready to start making more complicated definitions.
    compile-file: forth/common/internal/chartype.fth
    compile-file: forth/c-gen/core/terminal.fth
    compile-file: forth/common/core/terminal.fth
    compile-file: forth/c-gen/file/fileprim.fth
    compile-file: forth/c-gen/file/modes.fth
    compile-file: forth/common/core/source.fth
    compile-file: forth/common/core/parse.fth
    compile-file: forth/common/internal/error.fth
    compile-file: forth/common/core/comments.fth

    compile-file: forth/common/core/number.fth

    compile-file: forth/common/core/state.fth
    compile-file: forth/c-gen/search/base.fth
    compile-file: forth/common/search/base.fth
    compile-file: forth/common/internal/namespace.fth
    compile-file: forth/common/internal/lookup.fth
    compile-file: forth/common/internal/tokens.fth
    compile-file: forth/common/internal/interpret.fth

    compile-file: forth/common/core/definers.fth
    compile-file: forth/common/core/evaluate.fth
>>> END_DIRECT // }
>>>
>>>
>>> char *init_forth_defs[] = {
    include-source-text: forth/common/core/compile.fth
    include-source-text: forth/common/core/format.fth
    include-source-text: forth/common/core/quit.fth
    include-source-text: forth/common/file/include.fth
    include-source-text: forth/common/search/find.fth
    include-source-text: forth/common/search/vocab.fth
    include-source-text: forth/common/tools/tools.fth
>>>     NULL,
>>> };
