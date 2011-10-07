\ ------  ------  ------  ------  ------  ------  ------
\ index:
\ (                     6.1.0080 CORE                   26
\ >IN                   6.1.0560 CORE                   31
\ ABORT                 6.1.0670 CORE                   32
\ ABORT"                6.1.0680 CORE                   32
\ CHAR                  6.1.0895 CORE                   35
\ EVALUATE              6.1.1360 CORE                   39
\ EXECUTE               6.1.1370 CORE                   39
\ IMMEDIATE             6.1.1710 CORE                   41
\ QUIT                  6.1.2050 CORE                   44
\ SOURCE                6.1.2216 CORE                   46
\ STATE                 6.1.2250 CORE                   46
\ WORD                  6.1.2450 CORE                   49
\ [                     6.1.2500 CORE                   49
\ ]                     6.1.2540 CORE                   50
\ .(                    6.2.0200 CORE EXT               51
\ PARSE                 6.2.2008 CORE EXT               57
\ REFILL                6.2.2125 CORE EXT               57
\ RESTORE-INPUT         6.2.2148 CORE EXT               57
\ SAVE-INPUT            6.2.2182 CORE EXT               58
\ SOURCE-ID             6.2.2218 CORE EXT               58
\ \                     6.2.2535 CORE EXT               60

\ ------  ------  ------  ------  ------  ------  ------
\ parsing and interpretation - required

\ SOURCE ( -- c-addr u )
\ WORD ( -- c-addr u )

VARIABLE >IN 0 >IN !

: ( ( "ccc<paren>" -- ) [CHAR] ) PARSE 2DROP ;

\ FIXME - what if name is empty?
: CHAR ( "<spaces>name" -- char ) BL PARSE DROP C@ ;

VARIABLE STATE
: [ FALSE STATE ! ; IMMEDIATE
: ] TRUE  STATE ! ;
[

\ EXECUTE ( i*x xt -- j*x )
\ IMMEDIATE ( -- )
\ EVALUATE ( i*x c-addr u -- j*x )
\ QUIT ( -- ) ( R: i*x -- )

: interpret
    BEGIN
    parse-word ?DUP WHILE		( name len )
	2DUP sfind ?DUP IF
	    1 <> STATE @ AND IF EXECUTE ELSE COMPILE, THEN
	ELSE
	    COUNT >NUMBER
	    -13 error
	THEN
    REPEAT
;

\ PARSE ( char "ccc<char>" -- c-addr u )
\ REFILL ( -- flag )
\ RESTORE-INPUT ( xn .. x1 n -- flag )
\ SAVE-INPUT ( -- xn .. x1 n )
\ SOURCE-ID ( -- 0 | -1 )

: \ ( "ccc<eol>" -- ) SOURCE >IN ! DROP ; IMMEDIATE
: .( ( "ccc<paren>" -- ) [CHAR] ) PARSE TYPE ; IMMEDIATE
