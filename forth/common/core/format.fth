\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  #                     6.1.0030 CORE
\  #>                    6.1.0040 CORE
\  #S                    6.1.0050 CORE
\  .                     6.1.0180 CORE
\  ."                    6.1.0190 CORE
\  <#                    6.1.0490 CORE
\  HOLD                  6.1.1670 CORE
\  SIGN                  6.1.2210 CORE
\  U.                    6.1.2320 CORE
\
\  .R                    6.2.0210 CORE EXT
\  U.R                   6.2.2330 CORE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------

1 cells 8 * 2 * 2 + chars allot align
here 1 cells allot			( hold-addr )
: <# ( -- ) 0 [ over ] literal ! ;
: HOLD ( char -- ) [ over ] literal dup @ 1+ 2dup swap ! chars - c! ;
: #> ( xd -- c-addr u ) 2drop [ over ] literal dup @ dup >r chars - r> ;
drop					( )

: >digit ( n -- char ) dup 9 > [ 'A' '9' 1+ - ] literal and + '0' + ;
: # ( ud1 -- ud2 ) 0 base @ um/mod >r base @ um/mod swap >digit hold r> ;
: #S ( ud1 -- ud2 ) begin # 2dup or 0= until ;
: SIGN ( n -- ) 0< if '-' hold then ;

: . <# bl hold dup abs 0 #s rot sign #> type ;
: ." postpone s" postpone type ; compile-special
: U. <# bl hold 0 #s #> type ;

: .R ( n1 n2 -- ) >r <# dup abs 0 #s rot sign #> r> over - spaces type ;
: U.R ( u n -- ) >r <# 0 #s #> r> over - spaces type ;
