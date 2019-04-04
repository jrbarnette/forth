\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  FORTH-WORDLIST     16.6.1.1595 SEARCH
\  GET-CURRENT        16.6.1.1643 SEARCH
\  SET-CURRENT        16.6.1.2195 SEARCH
\ ------  ------  ------  ------  ------  ------  ------  ------

<C> &DICT.forth_wordlist;
constant FORTH-WORDLIST

: GET-CURRENT <C> &DICT.current; @ ;
: SET-CURRENT <C> &DICT.current; ! ;
