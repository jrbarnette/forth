\  Copyright 2020, by J. Richard Barnette. All Rights Reserved.

HOST-MODE

: OPEN-SOURCE-FILE ( "filename" -- fileid )
    parse-name 12 spaces ." /* ==== " 2dup type ."  ==== */" cr
    r/o open-file abort" failed to open file" ;


METADICT-HOST-MODE

: COMPILE-META: ( "filename" -- )
    open-source-file metadict-target-mode
    ['] interpret ['] interpret-file catch
    metadict-host-mode throw ;


HOST-MODE

: COMPILE-TARGET: ( "filename" -- )
    open-source-file target-mode
    ['] interpret ['] interpret-file catch
    host-mode throw ;


: WRITE-SOURCE-LINE  4 spaces source .c-string ',' emit cr ;
: INCLUDE-SOURCE-TEXT: ( "filename" -- )
    open-source-file ['] write-source-line interpret-file ;
