\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.

: EVALUATE ( i*x c-addr u -- j*x )
    nest-source source<evaluate interpret unnest-source ;
