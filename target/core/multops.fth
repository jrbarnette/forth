\ ------  ------  ------  ------  ------  ------  ------
\ index:
\ *                     6.1.0090 CORE                   26
\ */                    6.1.0100 CORE                   26
\ */MOD                 6.1.0110 CORE                   26
\ /                     6.1.0230 CORE                   28
\ /MOD                  6.1.0240 CORE                   28
\ FM/MOD                6.1.1561 CORE                   40
\ M*                    6.1.1810 CORE                   42
\ MOD                   6.1.1890 CORE                   43
\ S>D                   6.1.2170 CORE                   45
\ SM/REM                6.1.2214 CORE                   46
\ UM*                   6.1.2360 CORE                   47
\ UM/MOD                6.1.2370 CORE                   48

\ ------  ------  ------  ------  ------  ------  ------
\ primitives for double-cell multiply and divide
\ FM/MOD ( d n1 -- n2 n3 )
\ M* ( n1 n2 -- d )
\ SM/REM ( d n1 -- n2 n3 )
\ UM* ( u1 u2 -- ud )
\ UM/MOD ( ud u1 -- u2 u3 )

: * ( n1 n2 -- n3 ) M* DROP ;
: S>D ( n -- d ) DUP 0< ;

: */MOD ( n1 n2 -- n3 n4 ) >R M* R> FM/MOD ;
: */ ( n1 n2 n3 -- n4 ) >R M* R> FM/MOD DROP ;
: /MOD ( n1 n2 -- n3 n4 ) >R S>D R> FM/MOD SWAP ;
: / ( n1 n2 -- n3 ) >R S>D R> FM/MOD SWAP DROP ;
: MOD ( n1 n2 -- n3 ) >R S>D R> FM/MOD DROP ;

\ : */MOD ( n1 n2 -- n3 n4 ) >R M* R> SM/REM ;
\ : */ ( n1 n2 n3 -- n4 ) >R M* R> SM/REM DROP ;
\ : /MOD ( n1 n2 -- n3 n4 ) >R S>D R> SM/REM SWAP ;
\ : / ( n1 n2 -- n3 ) >R S>D R> SM/REM SWAP DROP ;
\ : MOD ( n1 n2 -- n3 ) >R S>D R> SM/REM DROP ;
