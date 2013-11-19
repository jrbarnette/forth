\ Copyright 2013, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\ primitives - stack manipulation:
\ ------  ------  ------  ------  ------  ------  ------  ------
\ 2DROP                 6.1.0370 CORE                   29
\ 2DUP                  6.1.0380 CORE                   29
\ 2OVER                 6.1.0400 CORE                   29
\ 2SWAP                 6.1.0430 CORE                   30
\ >R                    6.1.0580 CORE                   32
\ ?DUP                  6.1.0630 CORE                   32
\ DEPTH                 6.1.1200 CORE                   36
\ DROP                  6.1.1260 CORE                   37
\ DUP                   6.1.1290 CORE                   37
\ OVER                  6.1.1990 CORE                   42
\ R>                    6.1.2060 CORE                   43
\ R@                    6.1.2070 CORE                   43
\ ROT                   6.1.2160 CORE                   44
\ SWAP                  6.1.2260 CORE                   45
\ 2>R                   6.2.0340 CORE EXT               50
\ 2R>                   6.2.0410 CORE EXT               50
\ 2R@                   6.2.0415 CORE EXT               50
\ PICK                  6.2.2030 CORE EXT               55
\ ROLL                  6.2.2150 CORE EXT               56


\ ------  ------  ------  ------  ------  ------  ------  ------
\ primitives - execution:
\ ------  ------  ------  ------  ------  ------  ------  ------
\ EXECUTE               6.1.1370 CORE                   38
\ EXIT                  6.1.1380 CORE                   38


\ ------  ------  ------  ------  ------  ------  ------  ------
\ primitives - single cell arithmetic and logical:
\ ------  ------  ------  ------  ------  ------  ------  ------
\ +                     6.1.0120 CORE                   26
\ -                     6.1.0160 CORE                   27
\ 0<                    6.1.0250 CORE                   28
\ 0=                    6.1.0270 CORE                   28
\ 1+                    6.1.0290 CORE                   28
\ 1-                    6.1.0300 CORE                   29
\ 2*                    6.1.0320 CORE                   29
\ 2/                    6.1.0330 CORE                   29
\ <                     6.1.0480 CORE                   30
\ =                     6.1.0530 CORE                   31
\ >                     6.1.0540 CORE                   31
\ AND                   6.1.0720 CORE                   33
\ INVERT                6.1.1720 CORE                   40
\ LSHIFT                6.1.1805 CORE                   41
\ NEGATE                6.1.1910 CORE                   42
\ OR                    6.1.1980 CORE                   42
\ RSHIFT                6.1.2162 CORE                   44
\ U<                    6.1.2340 CORE                   46
\ XOR                   6.1.2490 CORE                   48
\ 0<>                   6.2.0260 CORE EXT               49
\ 0>                    6.2.0280 CORE EXT               50
\ <>                    6.2.0500 CORE EXT               50
\ U>                    6.2.2350 CORE EXT               57
\ ------  ------  ------  ------  ------  ------  ------  ------


\ ------  ------  ------  ------  ------  ------  ------  ------
\ primitives - single and double cell multiply and divide:
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


\ ------  ------  ------  ------  ------  ------  ------  ------
\ primitives - terminal I/O
\ ------  ------  ------  ------  ------  ------  ------  ------
\ EMIT                  6.1.1320 CORE                   38
\ KEY                   6.1.1750 CORE                   40
\ ------  ------  ------  ------  ------  ------  ------  ------


\ ------  ------  ------  ------  ------  ------  ------  ------
\ primitives - memory operations:
\ ------  ------  ------  ------  ------  ------  ------  ------
\ !                     6.1.0010 CORE                   25
\ @                     6.1.0650 CORE                   32
\ C!                    6.1.0850 CORE                   34
\ C@                    6.1.0870 CORE                   34
\ FILL                  6.1.1540 CORE                   39
\ MOVE                  6.1.1900 CORE                   42
\ ERASE                 6.2.1350 CORE EXT               53
\ ------  ------  ------  ------  ------  ------  ------  ------


\ ------  ------  ------  ------  ------  ------  ------  ------
\ machine specific - memory operations:
\ ------  ------  ------  ------  ------  ------  ------  ------
\ ALIGNED               6.1.0706 CORE                   33
\ CELL+                 6.1.0880 CORE                   35
\ CELLS                 6.1.0890 CORE                   35
\ CHAR+                 6.1.0897 CORE                   35
\ CHARS                 6.1.0898 CORE                   35
\ ------  ------  ------  ------  ------  ------  ------  ------


\ ------  ------  ------  ------  ------  ------  ------  ------
\ dictionary data space:
\ ------  ------  ------  ------  ------  ------  ------  ------
\ ,                     6.1.0150 CORE                   27
\ ALIGN                 6.1.0705 CORE                   33
\ ALLOT                 6.1.0710 CORE                   33
\ C,                    6.1.0860 CORE                   34
\ HERE                  6.1.1650 CORE                   39
\ UNUSED                6.2.2395 CORE EXT               57
\ ------  ------  ------  ------  ------  ------  ------  ------

\ non-standard
variable HERE-ADDR

: HERE ( -- addr ) here-addr @ ;
: ALLOT ( n -- ) here-addr @ + here-addr ! ;
: ALIGN ( -- ) here aligned here-addr ! ;
: , ( x -- ) here [ 1 cells ] literal allot ! ;
: C, ( char -- ) here [ 1 chars ] literal allot c! ;


\ ------  ------  ------  ------  ------  ------  ------  ------
\ basic utility:
\ ------  ------  ------  ------  ------  ------  ------  ------
\ BL                    6.1.0770 CORE                   34
\ ENVIRONMENT?          6.1.1345 CORE                   38
\ LITERAL               6.1.1780 CORE                   41
\ FALSE                 6.2.1485 CORE EXT               54
\ PAD                   6.2.2000 CORE EXT               55
\ TRUE                  6.2.2298 CORE EXT               57
\ ------  ------  ------  ------  ------  ------  ------  ------

\ implementation primitive: DO-LITERAL

20 constant BL
: LITERAL ( x -- ) postpone do-literal , ; compile-only


\ ------  ------  ------  ------  ------  ------  ------  ------
\ stack manipulation:
\ ------  ------  ------  ------  ------  ------  ------  ------
\ NIP                   6.2.1930 CORE EXT               54
\ TUCK                  6.2.2300 CORE EXT               57
\ ------  ------  ------  ------  ------  ------  ------  ------

: NIP ( x1 x2 -- x2 ) swap drop ;
: TUCK ( x1 x2 -- x2 x1 x2 ) swap over ;


\ ------  ------  ------  ------  ------  ------  ------  ------
\ single cell arithmetic and logical:
\ ------  ------  ------  ------  ------  ------  ------  ------
\ ABS                   6.1.0690 CORE                   32
\ MAX                   6.1.1870 CORE                   42
\ MIN                   6.1.1880 CORE                   42
\ S>D                   6.1.2170 CORE                   44
\ WITHIN                6.2.2440 CORE EXT               58
\ ------  ------  ------  ------  ------  ------  ------  ------

: ABS ( n -- u ) dup 0< if negate then ;
: MAX ( n1 n2 -- n3 ) 2dup < if swap then drop ;
: MIN ( n1 n2 -- n3 ) 2dup > if swap then drop ;
: S>D ( n -- d ) dup 0< ;

: WITHIN ( x1 x2 x3 -- flag ) over - >r - r> u< ;


\ ------  ------  ------  ------  ------  ------  ------  ------
\ memory operations:
\ ------  ------  ------  ------  ------  ------  ------  ------
\ +!                    6.1.0130 CORE                   27
\ 2!                    6.1.0310 CORE                   29
\ 2@                    6.1.0350 CORE                   29
\ COUNT                 6.1.0980 CORE                   36
\ ------  ------  ------  ------  ------  ------  ------  ------

: +! ( x a-addr -- ) dup >r @ + r> ! ;
: 2! ( x1 x2 a-addr -- ) swap over ! cell+ ! ;
: 2@ ( a-addr -- x1 x2 ) dup cell+ @ swap @ ;
: COUNT ( c-addr -- c-addr u ) dup char+ swap c@ ;


\ ------  ------  ------  ------  ------  ------  ------  ------
\ parsing:
\ ------  ------  ------  ------  ------  ------  ------  ------
\ >IN                   6.1.0560 CORE                   31
\ SOURCE                6.1.2216 CORE                   45
\ PARSE                 6.2.2008 CORE EXT               55
\ REFILL                6.2.2125 CORE EXT               55
\ RESTORE-INPUT         6.2.2148 CORE EXT               56
\ SAVE-INPUT            6.2.2182 CORE EXT               56
\ SOURCE-ID             6.2.2218 CORE EXT               56
\ ------  ------  ------  ------  ------  ------  ------  ------

variable >IN
    3 cells allot
: SOURCE ( -- c-addr u ) [ >in 2 cells + ] literal 2@ ;
: SOURCE-ID [ >in cell+ ] literal @ ;

: PARSE ( char "ccc<char>" -- c-addr u )
    >r source swap >in chars @ + >r		( R: delim c-addr )
    >in @ - -1
    begin 1+ 2dup > while			( end idx )
	dup 2r@ rot chars + c@			( end idx delim c )
	over bl = if nip 127 33 within else = then
    until
	nip dup 1+
    then >in +! r> swap r> drop
;

\ non-standard definitions
: SOURCE! ( c-addr u -- ) [ >in 2 cells + ] literal 2! ;
: SOURCE-ID! [ >in cell+ ] literal ! ;
: PARSE-WORD  ( char "<chars>ccc<char>" -- c-addr u )
    >r source swap >r				( R: delim c-addr )
    >in @ 1-
    begin 1+ 2dup > while			( end idx )
	dup 2r@ rot chars + c@			( end idx char c )
	over bl = if nip 33 127 within else <> then
    until then >in ! drop 2r> drop parse
;

\ implementation primitive: DO-ACCEPT ( c-addr +n1 -- +n2 eof? )

here 256 dup chars allot
: REFILL ( -- flag )
    ( C: tib #tib colon-sys )
    source-id if false exit then
    [ rot ] literal dup [ swap ] literal	( c-addr c-addr +n1 )
    do-accept if drop false exit then		( c-addr +n2 )
    source! 0 >in true
;


\ ------  ------  ------  ------  ------  ------  ------  ------
\ special syntax:
\ ------  ------  ------  ------  ------  ------  ------  ------
\ (                     6.1.0080 CORE                   26
\ CHAR                  6.1.0895 CORE                   35
\ S"                    6.1.2165 CORE                   44
\ [CHAR]                6.1.2520 CORE                   49
\ .(                    6.2.0200 CORE EXT               49
\ C"                    6.2.0855 CORE EXT               52
\ \                     6.2.2535 CORE EXT               58
\ ------  ------  ------  ------  ------  ------  ------  ------


: \ ( "ccc<eol>" -- ) source >in ! drop ; immediate
: CHAR ( "<spaces>name" -- char ) bl parse-word drop c@ ;
: [CHAR] char postpone literal ; compile-only
: ( [char] ) parse 2drop ; immediate
: .( [char] ) parse type ; compile-only

: COMPILE-STRING ( xt -- )
    [char] " parse dup execute
    here swap chars dup allot move align
;

\ implementation primitive: DO-S"
: S" ( "ccc<quote>" -- )
    postpone do-s" ['] , compile-string
; compile-only

\ implementation primitive: DO-C"
: C" ( "ccc<quote>" -- )
    postpone do-c" ['] c, compile-string
; compile-only


\ ------  ------  ------  ------  ------  ------  ------  ------
\ name definition and lookup:
\ ------  ------  ------  ------  ------  ------  ------  ------
\ '                     6.1.0070 CORE                   25
\ :                     6.1.0450 CORE                   30
\ ;                     6.1.0460 CORE                   30
\ >BODY                 6.1.0550 CORE                   31
\ CONSTANT              6.1.0950 CORE                   35
\ CREATE                6.1.1000 CORE                   36
\ DOES>                 6.1.1250 CORE                   37
\ FIND                  6.1.1550 CORE                   39
\ IMMEDIATE             6.1.1710 CORE                   40
\ RECURSE               6.1.2120 CORE                   43
\ VARIABLE              6.1.2410 CORE                   47
\ WORD                  6.1.2450 CORE                   48
\ [']                   6.1.2510 CORE                   48
\ :NONAME               6.2.0455 CORE EXT               51
\ MARKER                6.2.1850 CORE EXT               54
\ TO                    6.2.2295 CORE EXT               57
\ VALUE                 6.2.2405 CORE EXT               58
\ ------  ------  ------  ------  ------  ------  ------  ------

: ' bl parse-word lookup dup if >xt else -13 throw then ;
: ['] ' postpone literal ;

: WORD ( char "<chars>ccc<char>" -- c-addr )
    parse-word here				( src u dst )
    dup >r					( R: c-addr )
    2dup c! char+ swap chars			( src dst' u' )
    2dup + >r move bl r> c! r>			( c-addr )
;

\ non-standard definitions
: NAME, ( "<spaces>name<space>" mcp -- name )
    align here swap				( name mcp )
    get-current @ ,				\ link
    bl word count chars aligned allot drop	\ id
    ,						\ mcp
;
: LINK-NAME ( name -- ) get-current ! ;
: CREATE-NAME ( "<spaces>name<space>" mcp -- ) name, link-name ;

\ implementation constant: mcp-:
\ implementation constant: mcp-constant
\ implementation constant: mcp-create
\ implementation constant: mcp-variable

: : ( C: "<spaces>name" -- colon-sys ) mcp-: name, ] ;
: ; ( C: colon-sys -- ) link-name postpone exit postpone [ ; compile-only

: RECURSE ( colon-sys -- colon-sys ) dup >xt compile, ; immediate

: CONSTANT ( "<spaces>name" x -- ) mcp-constant create-name , ;
: VARIABLE ( "<spaces>name" -- ) mcp-variable create-name 0 , ;
: CREATE ( "<spaces>name" -- )
    mcp-create create-name [ here 3 cells + ] literal , ;
: DOES> ( C: colon-sys2 -- colon-sys2 )
    postpone exit here over >xt cell+ !
; immediate
: >BODY ( xt -- a-addr ) [ 2 cells ] literal + ;


\ ------  ------  ------  ------  ------  ------  ------  ------
\ execution flow of control:
\ ------  ------  ------  ------  ------  ------  ------  ------
\ +LOOP                 6.1.0140 CORE                   27
\ BEGIN                 6.1.0760 CORE                   34
\ DO                    6.1.1240 CORE                   36
\ ELSE                  6.1.1310 CORE                   37
\ I                     6.1.1680 CORE                   39
\ IF                    6.1.1700 CORE                   40
\ J                     6.1.1730 CORE                   40
\ LEAVE                 6.1.1760 CORE                   41
\ LOOP                  6.1.1800 CORE                   41
\ REPEAT                6.1.2140 CORE                   44
\ THEN                  6.1.2270 CORE                   46
\ UNLOOP                6.1.2380 CORE                   47
\ UNTIL                 6.1.2390 CORE                   47
\ WHILE                 6.1.2430 CORE                   47
\ ?DO                   6.2.0620 CORE EXT               51
\ AGAIN                 6.2.0700 CORE EXT               51
\ CASE                  6.2.0873 CORE EXT               52
\ ENDCASE               6.2.1342 CORE EXT               53
\ ENDOF                 6.2.1343 CORE EXT               53
\ OF                    6.2.1950 CORE EXT               54
\ ------  ------  ------  ------  ------  ------  ------  ------

\ ------  ------  ------  ------  ------  ------  ------  ------
\ number conversion:
\ ------  ------  ------  ------  ------  ------  ------  ------
\ >NUMBER               6.1.0570 CORE                   31
\ BASE                  6.1.0750 CORE                   34
\ DECIMAL               6.1.1170 CORE                   36
\ HEX                   6.2.1660 CORE EXT               54
\ ------  ------  ------  ------  ------  ------  ------  ------

variable BASE
: DECIMAL [ 1 dup + dup + 1 + dup + ] literal base ! ;

: >NUMBER ( ud1 c-addr1 u1 -- ud2 c-addr2 u2 )
    begin dup while			( ud c-addr u )
	>r dup >r c@			( ud char ) ( R: u c-addr )
	[char] 0 -
	9 over u< if [ char 0 char A - ] literal + then
	dup base @ u< invert if drop r> r> exit then
	>r base @ * >r base @ um* r> + r>
	rot dup >r + dup r> u< negate rot +
	r> char+ r> 1-
    repeat
;

: HEX 16 base ! ;


\ ------  ------  ------  ------  ------  ------  ------  ------
\ terminal:
\ ------  ------  ------  ------  ------  ------  ------  ------
\ ."                    6.1.0190 CORE                   28
\ ACCEPT                6.1.0695 CORE                   33
\ CR                    6.1.0990 CORE                   36
\ SPACE                 6.1.2220 CORE                   45
\ SPACES                6.1.2230 CORE                   45
\ TYPE                  6.1.2310 CORE                   46
\ ------  ------  ------  ------  ------  ------  ------  ------

: ACCEPT do-accept drop ;
: CR 10 emit ;
: SPACE bl emit ;

: SPACES ( n -- ) dup 0> if 0 do space loop else drop then ;
: TYPE ( c-addr u -- )
    chars over + swap ?do i c@ emit [ 1 chars ] literal +loop
;
: ." postpone s" type ; immediate


\ ------  ------  ------  ------  ------  ------  ------  ------
\ interpretation:
\ ------  ------  ------  ------  ------  ------  ------  ------
\ ABORT                 6.1.0670 CORE                   32
\ ABORT"                6.1.0680 CORE                   32
\ EVALUATE              6.1.1360 CORE                   38
\ POSTPONE              6.1.2033 CORE                   43
\ QUIT                  6.1.2050 CORE                   43
\ STATE                 6.1.2250 CORE                   45
\ [                     6.1.2500 CORE                   48
\ ]                     6.1.2540 CORE                   49
\ COMPILE,              6.2.0945 CORE EXT               52
\ [COMPILE]             6.2.2530 CORE EXT               58
\ ------  ------  ------  ------  ------  ------  ------  ------

\ implementation primitive: CLEARSTACK ( i*x -- )
\ implementation primitive: CLEARRSTACK ( R: i*x -- )

variable STATE
: [ false state ! ; immediate
: ] true  state ! ;

: INTERPRET
    begin parse-word dup while			( str len )
	2dup lookup if				( str len xt flags )
	    \ compile or execute a definition
	    2swap 2drop state @ if
		nf-immediate and if execute else compile, then
	    else
		nf-nointerp and if -14 throw else execute then
	    then
	else					( str len )
	    \ try to convert a number
	    over c@ [char] - = >r		( R: neg? )
	    r@ + swap r@ - swap			\ adjust for sign char
	    0 dup 2swap >number			( ul uh str len )
	    if -13 throw then 2drop		( ul )
	    r> if negate then			( n )
	    state @ if postpone literal then
	then
    repeat
;

: [COMPILE] ' compile, ; immediate
: POSTPONE
    parse-word lookup if
	nf-immediate and if
	    compile,
	else
	    [compile] literal [compile] compile,
	then
    else
	-13 throw
    then
; immediate

: EVALUATE ( i*x c-addr u -- j*x )
    \ save current source specification
    >in @ >r source-id >r source 2>r
    \ set current source to the string
    source! -1 source-id! 0 >in !
    interpret
    \ restore previous source specification
    2r> source! r> source-id! r> >in !
;

: QUIT
    clearrstack 0 >in cell+ !		\ TO SOURCE-ID
    postpone [ begin state @ 0= if ." ok " then
	refill while interpret
    repeat bye
;

: ABORT clearstack quit ;
: ABORT" postpone if postpone ." postpone abort postpone then ; compile-only


\ ------  ------  ------  ------  ------  ------  ------  ------
\ pictured string formatting:
\ ------  ------  ------  ------  ------  ------  ------  ------
\ #                     6.1.0030 CORE                   25
\ #>                    6.1.0040 CORE                   25
\ #S                    6.1.0050 CORE                   25
\ <#                    6.1.0490 CORE                   31
\ HOLD                  6.1.1670 CORE                   39
\ SIGN                  6.1.2210 CORE                   45
\ ------  ------  ------  ------  ------  ------  ------  ------

\ 2*n + 2 charcters, n = 8 * cell-size
\ (2 * 8 * cell-size + 2) characters
1 cells 8 * 2 * 2 + chars allot align here 1 cells allot
    constant HOLD-COUNT
: HOLD-POINTER ( -- c-addr ) hold-count dup @ chars - ;

: <# ( -- ) 0 hold-count ! ;
: HOLD ( char -- ) 1 hold-count +! hold-pointer c! ;
: #> ( xd -- c-addr u ) 2drop hold-pointer hold-count @ ;

: SIGN ( n -- ) 0< if [char] - hold then ;
: # ( ud1 -- ud2 )
    0 base @ um/mod rot swap base @ um/mod
    dup 10 u< if [char] 0 else [ char A 10 - ] literal then
    + hold swap
;

: #S ( ud1 -- ud2 ) begin # 2dup or 0= until ;


\ ------  ------  ------  ------  ------  ------  ------  ------
\ formatted output:
\ ------  ------  ------  ------  ------  ------  ------  ------
\ .                     6.1.0180 CORE                   27
\ U.                    6.1.2320 CORE                   46
\ .R                    6.2.0210 CORE EXT               49
\ U.R                   6.2.2330 CORE EXT               57
\ ------  ------  ------  ------  ------  ------  ------  ------

: . <# bl hold dup abs 0 #s rot sign #> type ;
: U. <# bl hold 0 #s #> type ;

: .R ( n1 n2 -- ) >r <# dup abs 0 #s rot sign #> r> over - spaces type ;
: U.R ( u n -- ) >r <# 0 #s #> r> over - spaces type ;


\ obsolete:
\ #TIB                  6.2.0060 CORE EXT               49
\ CONVERT               6.2.0970 CORE EXT               52
\ EXPECT                6.2.1390 CORE EXT               53
\ QUERY                 6.2.2040 CORE EXT               55
\ SPAN                  6.2.2240 CORE EXT               56
\ TIB                   6.2.2290 CORE EXT               56
