\ Copyright 2020, by J. Richard Barnette, All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  CATCH               9.6.1.0875 EXCEPTION
\ ------  ------  ------  ------  ------  ------  ------  ------

: CATCH ( i*x xt -- j*x 0 | i*x n )
    do-catch ?dup 0= if execute drop-catch 0 then ;
