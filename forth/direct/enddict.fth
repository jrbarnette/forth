\  Copyright 2017, by J. Richard Barnette. All Rights Reserved.

<HOST> QUIT DEFINITIONS

create source-line 256 dup chars allot constant line-size

: source: ( "filename" -- )
    '"' parse r/o open-file abort" failed to open file"
    begin dup >r source-line dup line-size r> read-line drop while
        4 spaces c-string ',' emit cr
    repeat 2drop drop ;

TARGET <DIRECT>
>>> END_DIRECT // }
>>>
>>>
>>> char *init_forth_defs[] = {
<HOST> QUIT
        source: bootstrap/stdforth.fth
TARGET <DIRECT>
>>>     NULL,
>>> };
