\  Copyright 2023, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  GET-CURRENT        16.6.1.1643 SEARCH
\  SET-CURRENT        16.6.1.2195 SEARCH
\ ------  ------  ------  ------  ------  ------  ------  ------

\ Storage for the compilation wordlist

\ from Forth-83
variable CURRENT

: GET-CURRENT current @ ;	( -- wid )
: SET-CURRENT current ! ;	( wid -- )
