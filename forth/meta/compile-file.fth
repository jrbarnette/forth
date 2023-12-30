\  Copyright 2020, by J. Richard Barnette. All Rights Reserved.

HOST-MODE definitions

: OPEN-SOURCE-FILE ( "filename" -- fileid )
    parse-name 12 spaces ." /* ==== " 2dup type ."  ==== */" cr
    r/o open-file abort" failed to open file" ;


META-HOST-MODE definitions

: INTERPRET-META-FILE
    meta-target-mode
    begin refill if ['] interpret catch ?dup else 0 -1 then until
    meta-host-mode source-id close-file drop throw ;
: COMPILE-META: ( "filename" -- )
    open-source-file ['] interpret-meta-file with-input-source ;


HOST-MODE definitions

: INTERPRET-TARGET-FILE
    target-mode
    begin refill if ['] interpret catch ?dup else 0 -1 then until
    host-mode source-id close-file drop throw ;
: COMPILE-TARGET: ( "filename" -- )
    open-source-file ['] interpret-target-file with-input-source ;


create SOURCE-LINE 256 dup chars allot 2 - constant LINE-SIZE
: read-source-line ( fileid -- line #line flag )
    >r source-line dup line-size r> read-line drop ;
: INCLUDE-SOURCE-TEXT: ( "filename" -- )
    open-source-file begin dup read-source-line while
	4 spaces .c-string ',' emit cr
    repeat 2drop close-file drop ;
