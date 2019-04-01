\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.
\ Internal code relating to creating definitions.

: STR, ( c-addr u -- )
    dup c, here swap chars dup allot move
    \ 0 do over i chars + c@ toupper over i chars + c! loop 2drop
;

: NAME, ( mcp wid "<spaces>name<space>" -- name )
    align here >r @ , parse-name str, align , r>
;
