\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  '                     6.1.0070 CORE
\  FIND                  6.1.1550 CORE
\
\  FIND               16.6.1.1550 SEARCH
\  SEARCH-WORDLIST    16.6.1.2192 SEARCH
\ ------  ------  ------  ------  ------  ------  ------  ------

: ' parse-name lookup ?dup 0= if -13 throw then name>xt ;
: FIND >r get-order r> lookup name>find ;
: SEARCH-WORDLIST wid-lookup name>find ;
