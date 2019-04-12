\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  FIND                  6.1.1550 CORE
\
\  FIND               16.6.1.1550 SEARCH
\  SEARCH-WORDLIST    16.6.1.2192 SEARCH
\ ------  ------  ------  ------  ------  ------  ------  ------

: FIND dup count lookup name>find dup if rot drop then ;
: SEARCH-WORDLIST wid-lookup name>find ;
