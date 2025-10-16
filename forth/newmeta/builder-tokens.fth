\  Copyright 2025, by J. Richard Barnette, All Rights Reserved.
\  Builder mode token handling

BUILDER-HOST-MODE

: INTERPRET-NAME ( i*x name -- j*x )
    name>xt+flags state @ if
	immediate? 2 +
    else
	compile-only? if -14 .error then 1
    then ref-mode ! execute ;

: INTERPRET-NUMBER ( x -- ) literal-cell builder-literal ;
