\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  EVALUATE              6.1.1360 CORE
\ ------  ------  ------  ------  ------  ------  ------  ------

: EVALUATE ( i*x c-addr u -- j*x )  -1 ['] interpret with-input-source ;
