\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\  names.fth - C internal functions and Forth standard words for
\    dictionary name space.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  '                     6.1.0070 CORE                   25
\  EXIT                  6.1.1380 CORE                   38
\  FIND                  6.1.1550 CORE                   39
\
\  SEARCH-WORDLIST    16.6.1.2192 SEARCH                120
\ ------  ------  ------  ------  ------  ------  ------  ------

prim: '                 x_tick

\ EXIT out of order for reference below

prim: FIND              x_find

prim: SEARCH-WORDLIST   x_search_wordlist
