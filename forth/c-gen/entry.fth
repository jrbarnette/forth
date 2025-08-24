\  Copyright 2020, by J. Richard Barnette. All Rights Reserved.

: entry: create , parse-name counted, does>
    '.' emit dup cell+ count type ."  = " @ execute ;

' type          entry: .exec handler ( c-addr u -- )
' type          entry: .meta ip ( c-addr u -- )
' .c-string     entry: .str id ( c-addr u -- )
' .c-cell       entry: .expr cell ( c-addr u -- )
' .c-hex        entry: .cell cell ( x -- )
