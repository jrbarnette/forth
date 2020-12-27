\ Copyright 2020, by J. Richard Barnette, All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  CATCH               9.6.1.0875 EXCEPTION
\ ------  ------  ------  ------  ------  ------  ------  ------

: CATCH ( ... ) do-catch ?dup 0= if execute undo-catch 0 then ;
