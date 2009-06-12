\ ------  ------  ------  ------  ------  ------  ------
\ primitives for double-cell multiply and divide
\ FM/MOD                6.1.1561 CORE                   40
\ M*                    6.1.1810 CORE                   42
\ SM/REM                6.1.2214 CORE                   46
\ UM*                   6.1.2360 CORE                   47
\ UM/MOD                6.1.2370 CORE                   48

\ *                     6.1.0090 CORE                   26
: * ( n1 n2 -- n3 ) m* drop ;

\ S>D                   6.1.2170 CORE                   45
: S>D ( n -- d ) dup 0< ;

\ */MOD                 6.1.0110 CORE                   26
\ : */MOD ( n1 n2 -- n3 n4 ) >r m* r> sm/rem ;
: */MOD ( n1 n2 -- n3 n4 ) >r m* r> fm/mod ;

\ /                     6.1.0230 CORE                   28
\ : / ( n1 n2 -- n3 ) >r s>d r> sm/rem swap drop ;
: / ( n1 n2 -- n3 ) >r s>d r> fm/mod swap drop ;

\ /MOD                  6.1.0240 CORE                   28
\ : /MOD ( n1 n2 -- n3 n4 ) >r s>d r> sm/rem swap ;
: /MOD ( n1 n2 -- n3 n4 ) >r s>d r> fm/mod swap ;

\ MOD                   6.1.1890 CORE                   43
\ : MOD ( n1 n2 -- n3 ) >r s>d r> sm/rem drop ;
: MOD ( n1 n2 -- n3 ) >r s>d r> fm/mod drop ;

\ */                    6.1.0100 CORE                   26
: */ ( n1 n2 n3 -- n4 ) */mod drop ;
