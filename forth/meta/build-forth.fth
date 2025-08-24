\  Copyright 2025, by J. Richard Barnette. All Rights Reserved.

: include-file: ( "filename" -- ) parse-name included ;

include-file: forth/meta/vocab.fth
include-file: forth/c-gen/c-data.fth
include-file: forth/c-gen/transcribe.fth
include-file: forth/c-gen/entry.fth

include-file: forth/meta/meta-host.fth
include-file: forth/meta/host.fth
include-file: forth/meta/target.fth

include-file: forth/meta/target-tokens.fth
include-file: forth/common/internal/interpret.fth

include-file: forth/meta/output-dict.fth
