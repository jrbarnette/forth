\  Copyright 2025, by J. Richard Barnette. All Rights Reserved.

only FORTH definitions

: INCLUDE-FILE: ( "filename" -- ) parse-name included ;

include-file: forth/meta/modes.fth

HOST-MODE
include-file: forth/c-gen/transcribe.fth
include-file: forth/c-gen/c-data.fth
include-file: forth/c-gen/entry.fth
include-file: forth/meta/metadict.fth

HOST-MODE
: OPEN-SOURCE-FILE ( "filename" -- fileid )
    parse-name 12 spaces ." /* ==== " 2dup type ."  ==== */" cr
    r/o open-file abort" failed to open file" ;

: COMPILE-FILE:  open-source-file include-file ;

METADICT-HOST-MODE

TRANSCRIBE>
#include <stddef.h>

#include "forth.h"
#include "cforth.h"
#include "dictionary.h"
#include "fileio.h"


static xt_ft references[];
static char *init_forth_defs[];

static
vmcode_ft meta_dictionary[] = {
    FORTH>
    start-entries
    HOST-MODE also METADICT-DEFINERS METADICT-DEFINITIONS
    compile-file: forth/c-gen/core/vmprim.fth
    compile-file: forth/c-gen/core/control.fth

    compile-file: forth/c-gen/core/stackprim.fth
    compile-file: forth/c-gen/core/arithprim.fth
    compile-file: forth/c-gen/core/memprim.fth
    compile-file: forth/c-gen/core/multprim.fth
    compile-file: forth/c-gen/exception/throwprim.fth

    include-file: forth/meta/meta-target.fth

    METADICT-SPECIAL-MODE
    include-file: forth/common/internal/branches.fth

    METADICT-DEFINITIONS
    include-file: forth/common/core/control.fth

    METADICT-SPECIAL-MODE
    include-file: forth/common/internal/doprolog.fth

    METADICT-DEFINITIONS
    include-file: forth/common/core/docase.fth

    \ Build up INTERPRET in the host side of the meta-dictionary, then
    \ create a definition to apply it to compiling files.
    METADICT-SPECIAL-MODE
    include-file: forth/common/internal/tokens.fth
    METADICT-HOST-MODE
    include-file: forth/common/internal/interpret.fth

    : COMPILE-META:
	open-source-file metadict-target-mode
	['] interpret ['] interpret-file catch
	metadict-host-mode throw ;

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
HOST-MODE

include-file: forth/meta/builder.fth

BUILDER-HOST-MODE
: meta-exit [metadict] exit ;
: meta-do-literal [metadict] do-literal ;

also METADICT-TARGET
TRANSCRIBE>
#define DO_LITERAL	` meta-do-literal `
#define LINKNAME	` link-name `
#define STARTNAME	` meta-startname `
#define ADDNAME		` meta-addname `
#define SETFLAGS	` meta-setflags `
#define INTERPRET	` meta-interpret `
#define COMPILE		` meta-compile `
#define REFERENCE	` meta-reference `
#define COMMA		` , `


FORTH>
previous

TRANSCRIBE>
vmcode_ft initialize_forth[] = {
    FORTH>
    BUILDER-HOST-MODE
    start-entries
    build-initialize

    also BUILDER-SHADOW
    also BUILDER-TARGET definitions previous
    compile-file: forth/c-gen/core/vmprim.fth
    compile-file: forth/c-gen/core/control.fth

    compile-file: forth/c-gen/core/stackprim.fth
    compile-file: forth/c-gen/core/arithprim.fth
    compile-file: forth/c-gen/core/memprim.fth
    compile-file: forth/c-gen/core/multprim.fth
    compile-file: forth/c-gen/exception/throwprim.fth

    BUILDER-HOST-MODE
    include-file: forth/meta/builder-target.fth
    include-file: forth/meta/builder-tokens.fth
    include-file: forth/common/internal/interpret.fth
    : COMPILE-BUILDER: ( "filename" -- )
	open-source-file builder-target-mode
	['] interpret ['] interpret-file catch
	builder-host-mode throw ;

    compile-builder: forth/c-gen/core/memops.fth
    compile-builder: forth/c-gen/core/allot.fth
    compile-builder: forth/common/core/memops.fth

    BUILDER-HOST-MODE
    include-file: forth/meta/builder-postpone.fth

    compile-builder: forth/common/core/literal.fth
    compile-builder: forth/common/core/pad.fth

    \ We want "arithops" early, but that depends on "control", which
    \ in turn depends on "stackops".
    compile-builder: forth/common/core/stackops.fth
    compile-builder: forth/common/internal/mark.fth
    compile-builder: forth/common/internal/branches.fth
    compile-builder: forth/common/core/control.fth
    compile-builder: forth/common/internal/markchain.fth
    compile-builder: forth/common/internal/doprolog.fth
    compile-builder: forth/common/core/docase.fth
    compile-builder: forth/common/core/arithops.fth

    compile-builder: forth/common/internal/ascii.fth
    compile-builder: forth/common/internal/namespace.fth
    compile-builder: forth/common/search/current.fth
    compile-builder: forth/common/internal/dictionary.fth
    compile-builder: forth/common/internal/wordlists.fth

    compile-builder: forth/common/exception/catch.fth

    \ At this point we have working definitions for "dictionary",
    \ "stack", "arith", "mem", "mult", and "control" words.  We're
    \ ready to start making more complicated definitions.
    compile-builder: forth/c-gen/core/terminal.fth
    compile-builder: forth/common/core/terminal.fth
    compile-builder: forth/c-gen/file/fileprim.fth
    compile-builder: forth/c-gen/file/modes.fth
    compile-builder: forth/common/core/source.fth
    compile-builder: forth/common/core/parse.fth
    compile-builder: forth/common/internal/error.fth

    compile-builder: forth/c-gen/search/base.fth
    compile-builder: forth/common/search/base.fth
    compile-builder: forth/common/internal/lookup.fth

    compile-builder: forth/common/core/number.fth

    compile-builder: forth/common/core/state.fth
    compile-builder: forth/common/internal/numparse.fth
    compile-builder: forth/common/internal/tokens.fth
    compile-builder: forth/common/internal/interpret.fth

    compile-builder: forth/c-gen/core/handlers.fth
    compile-builder: forth/common/core/definers.fth

    compile-builder: forth/common/core/comments.fth

    compile-builder: forth/common/core/evaluate.fth
    0 ref-mode ! ' EVALUATE execute
    also METADICT-TARGET
    .start interpret-lines .end
    previous
    .start meta-exit .end
    TRANSCRIBE>
};


FORTH>
HOST-MODE
: WRITE-SOURCE-LINE  4 spaces source .c-string ',' emit cr ;
: INCLUDE-SOURCE-TEXT: ( "filename" -- )
    open-source-file ['] write-source-line interpret-file ;


TRANSCRIBE>
static
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

#define NREFERENCE ` BUILDER-HOST-MODE nref @ .c-decimal HOST-MODE `
static xt_ft references[NREFERENCE] = { NULL };
FORTH>

\ include-file: reference/forth/showwords.fth
