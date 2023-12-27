\  Copyright 2020, by J. Richard Barnette. All Rights Reserved.

vocabulary HOST

: HOST-MODE only FORTH also HOST ;
HOST-MODE definitions

vocabulary BUILDER
vocabulary BUILDER-DEFS
vocabulary BUILDER-INTERP
vocabulary BUILDER-SPECIAL

: BUILDER-INTERP-MODE
    only FORTH also BUILDER-INTERP
    also BUILDER-DEFS definitions previous ;

: BUILDER-COMPILE-MODE
    HOST-MODE also BUILDER-DEFS definitions previous
    also BUILDER-SPECIAL ;

' builder-defs >body constant builder-defs-wordlist
' builder-special >body constant builder-special-wordlist

\ : do-in-wordlist ( creator-xt wid "name" -- )
\     get-current >r set-current execute r> set-current ;
