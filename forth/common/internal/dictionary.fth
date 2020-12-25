\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

: CHARS, ( c-addr u -- ) here swap chars dup allot move ;
: COUNTED, ( c-addr u -- ) dup c, chars, ;
