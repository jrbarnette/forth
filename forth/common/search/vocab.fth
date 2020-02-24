\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.

\ Standard words:
\ ------  ------  ------  ------  ------  ------  ------  ------
\  WORDLIST           16.6.1.2460 SEARCH
\  ALSO               16.6.2.0715 SEARCH EXT
\  FORTH              16.6.2.1590 SEARCH EXT
\  ONLY               16.6.2.1965 SEARCH EXT
\  PREVIOUS           16.6.2.2037 SEARCH EXT
\ ------  ------  ------  ------  ------  ------  ------  ------
\ Non-standard words:
\ ------  ------  ------  ------  ------  ------  ------  ------
\  VOCABULARY         FORTH-83
\ ------  ------  ------  ------  ------  ------  ------  ------

: WORDLIST ( -- wid ) here 0 , ;

: do-vocabulary  get-order-first-entry ! ;
: VOCABULARY create 0 , does> do-vocabulary ;
: FORTH ( -- ) forth-wordlist do-vocabulary ;

: ALSO ( -- ) get-order ?dup if over swap 1+ set-order else -50 throw then ;
: ONLY ( -- ) -1 set-order ;
: PREVIOUS ( -- ) get-order ?dup if nip 1- set-order else -50 throw then ;
