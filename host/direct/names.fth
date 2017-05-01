\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\  names.fth - C internal functions and Forth standard words for
\    dictionary name space.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  '                     6.1.0070 CORE                   25
\  :                     6.1.0450 CORE                   30
\  ;                     6.1.0460 CORE                   30
\  >BODY                 6.1.0550 CORE                   31
\  CONSTANT              6.1.0950 CORE                   35
\  CREATE                6.1.1000 CORE                   36
\  DOES>                 6.1.1250 CORE                   37
\  EXIT                  6.1.1380 CORE                   38
\  FIND                  6.1.1550 CORE                   39
\  VARIABLE              6.1.2410 CORE                   47
\
\  FORTH-WORDLIST     16.6.1.1595 SEARCH                119
\  SEARCH-WORDLIST    16.6.1.2192 SEARCH                120
\ ------  ------  ------  ------  ------  ------  ------  ------

expr do_does; expr DOES_XT; !

prim: '                 x_tick
prim: :                 x_colon

\ EXIT out of order for reference below
prim: EXIT              x_exit         no-interpret

prim: ;                 x_semicolon    compile-only
[compile] EXIT

prim: >BODY             x_to_body
prim: CONSTANT          x_constant
prim: CREATE            x_create
prim: DOES>             x_does         compile-only
prim: FIND              x_find
prim: VARIABLE          x_variable

prim: FORTH-WORDLIST    x_forth_wordlist
prim: SEARCH-WORDLIST   x_search_wordlist
