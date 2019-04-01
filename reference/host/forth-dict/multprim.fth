\ Copyright 2015, by J. Richard Barnette, All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  *                     6.1.0090 CORE                   26
\  */                    6.1.0100 CORE                   26
\  */MOD                 6.1.0110 CORE                   26
\  /                     6.1.0230 CORE                   28
\  /MOD                  6.1.0240 CORE                   28
\  FM/MOD                6.1.1561 CORE                   39
\  M*                    6.1.1810 CORE                   41
\  MOD                   6.1.1890 CORE                   42
\  SM/REM                6.1.2214 CORE                   45
\  UM*                   6.1.2360 CORE                   46
\  UM/MOD                6.1.2370 CORE                   46
\ ------  ------  ------  ------  ------  ------  ------  ------

\   FM/MOD
\   SM/REM
\ : * M* DROP ;
\ : */MOD >R M* R> FM/MOD ;
\ : */MOD >R M* R> SM/REM ;
\ : */ */MOD SWAP DROP ;
\ : /MOD >R S>D R> FM/MOD ;
\ : /MOD >R S>D R> SM/REM ;
\ : / /MOD SWAP DROP ;

prim: *
\     */        missing
\     */MOD     missing
prim: /
prim: /MOD
\     FM/MOD    missing
prim: M*
prim: MOD
\     SM/REM    missing
prim: UM*
prim: UM/MOD
