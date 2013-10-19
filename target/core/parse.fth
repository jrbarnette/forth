\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\ (                     6.1.0080 CORE                   26
\ >IN                   6.1.0560 CORE                   31
\ CHAR                  6.1.0895 CORE                   35
\ SOURCE                6.1.2216 CORE                   45
\ WORD                  6.1.2450 CORE                   48
\ ------  ------  ------  ------  ------  ------  ------  ------

\ anonymous definitions
\ label word-addr - >= 33 characters, <= size of counted string
\ skip-delimiters ( char "<chars>" -- )
: parse-name ( -- c-addr u ) bl skip-delimiters bl parse ;

variable >IN
: CHAR ( "<spaces>name" --- char ) parse-name drop c@ ;
: ( [char] ) parse 2drop ; immediate
: WORD ( char "<chars>ccc<char>" -- c-addr )
    parse-name word-addr 2dup c! char+
    swap chars 2dup + bl c! move ;

\ ------  ------  ------  ------  ------  ------  ------  ------
\ .(                    6.2.0200 CORE EXT               49
\ PARSE                 6.2.2008 CORE EXT               55
\ REFILL                6.2.2125 CORE EXT               55
\ RESTORE-INPUT         6.2.2148 CORE EXT               56
\ SAVE-INPUT            6.2.2182 CORE EXT               56
\ SOURCE-ID             6.2.2218 CORE EXT               56
\ \                     6.2.2535 CORE EXT               58
\ ------  ------  ------  ------  ------  ------  ------  ------

\ input source specification
\   common data -
\     source-id
\     start address
\     current size
\     >in
\   terminal extends common data -
\     max size
\   seekable file extends terminal -
\     offset of buffer w/in file
\   block data extends common data -
\     ???
\ REFILL depends on input source
: .( [char] ) parse type ; immediate
: \ source >in ! drop ; immediate
