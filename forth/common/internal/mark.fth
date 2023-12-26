\ Copyright 2023, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  <MARK		FORTH-83 System Extension Word Set
\  <RESOLVE		FORTH-83 System Extension Word Set
\  >MARK		FORTH-83 System Extension Word Set
\  >RESOLVE		FORTH-83 System Extension Word Set
\ ------  ------  ------  ------  ------  ------  ------  ------


\ FORTH-83 - System Extension Word Set, Compiler Layer
: <MARK ( -- dest ) here ; compile-only
: <RESOLVE ( dest -- ) here - [ 1 cells ] literal / , ; compile-only
: >MARK ( -- orig ) here [ 1 cells ] literal allot ; compile-only
: >RESOLVE ( orig -- ) here over - [ 1 cells ] literal / swap ! ; compile-only
