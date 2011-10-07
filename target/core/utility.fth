\ ------  ------  ------  ------  ------  ------  ------
\ index:
\ BL                    6.1.0770 CORE                   34
\ COUNT                 6.1.0980 CORE                   36
\ ENVIRONMENT?          6.1.1345 CORE                   38
\ FALSE                 6.2.1485 CORE EXT               55
\ TRUE                  6.2.2298 CORE EXT               59
\ ------  ------  ------  ------  ------  ------  ------

20 CONSTANT BL
1 0= CONSTANT FALSE
0 0= CONSTANT TRUE

: COUNT ( c-addr -- c-addr u ) DUP CHAR+ SWAP C@ ;

\ ENVIRONMENT ( c-addr u -- false | i*x true )
