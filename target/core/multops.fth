\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\ *                     6.1.0090 CORE                   26
\ */                    6.1.0100 CORE                   26
\ */MOD                 6.1.0110 CORE                   26
\ /                     6.1.0230 CORE                   28
\ /MOD                  6.1.0240 CORE                   28
\ FM/MOD                6.1.1561 CORE                   39
\ M*                    6.1.1810 CORE                   41
\ MOD                   6.1.1890 CORE                   42
\ SM/REM                6.1.2214 CORE                   45
\ UM*                   6.1.2360 CORE                   46
\ UM/MOD                6.1.2370 CORE                   46
\ ------  ------  ------  ------  ------  ------  ------  ------

\ primitives for double-cell multiply and divide
\ FM/MOD ( d n1 -- n2 n3 )
\ M* ( n1 n2 -- d )
\ SM/REM ( d n1 -- n2 n3 )
\ UM* ( u1 u2 -- ud )
\ UM/MOD ( ud u1 -- u2 u3 )

\ primitives - could be in forth
\ : * ( n1 n2 -- n3 ) m* drop ;
\ : / ( n1 n2 -- n3 ) >r s>d r> sm/rem swap drop ;
\ : / ( n1 n2 -- n3 ) >r s>d r> fm/mod swap drop ;
\ : MOD ( n1 n2 -- n3 ) >r s>d r> fm/mod drop ;

: */MOD ( n1 n2 -- n3 n4 ) >r m* r> fm/mod ;
: */ ( n1 n2 n3 -- n4 ) >r m* r> fm/mod drop ;
: /MOD ( n1 n2 -- n3 n4 ) >r s>d r> fm/mod swap ;

\ : */MOD ( n1 n2 -- n3 n4 ) >r m* r> sm/rem ;
\ : */ ( n1 n2 n3 -- n4 ) >r m* r> sm/rem drop ;
\ : /MOD ( n1 n2 -- n3 n4 ) >r s>d r> sm/rem swap ;
