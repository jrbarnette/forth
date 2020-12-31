\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.

: INTERPRET-STRING ( i*x c-addr u -- j*x )  source<evaluate interpret ;
: EVALUATE ( i*x c-addr u -- j*x )  ['] interpret-string with-nested-source ;
