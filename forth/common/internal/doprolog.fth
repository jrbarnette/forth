\ Copyright 2023, by J. Richard Barnette. All Rights Reserved.

: branch>chain ( -- ) >branch mark>chain ; compile-only

: do-prolog postpone do-do start-chain ; compile-only
: do-epilog postpone begin ; compile-only
