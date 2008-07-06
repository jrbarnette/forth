/*
 * Copyright 2008, by J. Richard Barnette
 */

#include "forth.h"

/* ! ( x a-addr -- ) */
static cell_ft
x_store(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    *(cell_ft *)tos = POP(vm);
    return POP(vm);
}

/* @ ( a-addr -- x ) */
static cell_ft
x_fetch(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    /* ( a-addr -- x ) */
    CHECK_POP(vm, 1);
    return *(cell_ft *)tos;
}

/* + ( x1 x2 -- x ) */
static cell_ft
x_plus(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 2);
    return tos + POP(vm);
}

defn_dt
primitive_defns[] = {
    { "!",     x_store },
    { "@",     x_fetch },
    { "+",     x_plus },
    { NULL }
};

#if 0
!                     6.1.0010 CORE                   25
#                     6.1.0030 CORE                   25
#>                    6.1.0040 CORE                   25
#S                    6.1.0050 CORE                   25
'                     6.1.0070 CORE                   25
(                     6.1.0080 CORE                   26
*                     6.1.0090 CORE                   26
*/                    6.1.0100 CORE                   26
*/MOD                 6.1.0110 CORE                   26
+                     6.1.0120 CORE                   26
+!                    6.1.0130 CORE                   27
+LOOP                 6.1.0140 CORE                   27
,                     6.1.0150 CORE                   27
-                     6.1.0160 CORE                   27
.                     6.1.0180 CORE                   27
."                    6.1.0190 CORE                   28
/                     6.1.0230 CORE                   28
/MOD                  6.1.0240 CORE                   28
0<                    6.1.0250 CORE                   28
0=                    6.1.0270 CORE                   28
1+                    6.1.0290 CORE                   28
1-                    6.1.0300 CORE                   29
2!                    6.1.0310 CORE                   29
2*                    6.1.0320 CORE                   29
2/                    6.1.0330 CORE                   29
2@                    6.1.0350 CORE                   29
2DROP                 6.1.0370 CORE                   29
2DUP                  6.1.0380 CORE                   29
2OVER                 6.1.0400 CORE                   29
2SWAP                 6.1.0430 CORE                   30
:                     6.1.0450 CORE                   30
;                     6.1.0460 CORE                   30
<                     6.1.0480 CORE                   30
<#                    6.1.0490 CORE                   31
=                     6.1.0530 CORE                   31
>                     6.1.0540 CORE                   31
>BODY                 6.1.0550 CORE                   31
>IN                   6.1.0560 CORE                   31
>NUMBER               6.1.0570 CORE                   31
>R                    6.1.0580 CORE                   32
?DUP                  6.1.0630 CORE                   32
@                     6.1.0650 CORE                   32
ABORT                 6.1.0670 CORE                   32
ABORT"                6.1.0680 CORE                   32
ABS                   6.1.0690 CORE                   32
ACCEPT                6.1.0695 CORE                   33
ALIGN                 6.1.0705 CORE                   33
ALIGNED               6.1.0706 CORE                   33
ALLOT                 6.1.0710 CORE                   33
AND                   6.1.0720 CORE                   33
BASE                  6.1.0750 CORE                   34
BEGIN                 6.1.0760 CORE                   34
BL                    6.1.0770 CORE                   34
C!                    6.1.0850 CORE                   34
C,                    6.1.0860 CORE                   34
C@                    6.1.0870 CORE                   34
CELL+                 6.1.0880 CORE                   35
CELLS                 6.1.0890 CORE                   35
CHAR                  6.1.0895 CORE                   35
CHAR+                 6.1.0897 CORE                   35
CHARS                 6.1.0898 CORE                   35
CONSTANT              6.1.0950 CORE                   35
COUNT                 6.1.0980 CORE                   36
CR                    6.1.0990 CORE                   36
CREATE                6.1.1000 CORE                   36
DECIMAL               6.1.1170 CORE                   36
DEPTH                 6.1.1200 CORE                   36
DO                    6.1.1240 CORE                   37
DOES>                 6.1.1250 CORE                   37
DROP                  6.1.1260 CORE                   37
DUP                   6.1.1290 CORE                   38
ELSE                  6.1.1310 CORE                   38
EMIT                  6.1.1320 CORE                   38
ENVIRONMENT?          6.1.1345 CORE                   38
EVALUATE              6.1.1360 CORE                   39
EXECUTE               6.1.1370 CORE                   39
EXIT                  6.1.1380 CORE                   39
FILL                  6.1.1540 CORE                   39
FIND                  6.1.1550 CORE                   39
FM/MOD                6.1.1561 CORE                   40
HERE                  6.1.1650 CORE                   40
HOLD                  6.1.1670 CORE                   40
I                     6.1.1680 CORE                   40
IF                    6.1.1700 CORE                   40
IMMEDIATE             6.1.1710 CORE                   41
INVERT                6.1.1720 CORE                   41
J                     6.1.1730 CORE                   41
KEY                   6.1.1750 CORE                   41
LEAVE                 6.1.1760 CORE                   41
LITERAL               6.1.1780 CORE                   42
LOOP                  6.1.1800 CORE                   42
LSHIFT                6.1.1805 CORE                   42
M*                    6.1.1810 CORE                   42
MAX                   6.1.1870 CORE                   42
MIN                   6.1.1880 CORE                   42
MOD                   6.1.1890 CORE                   43
MOVE                  6.1.1900 CORE                   43
NEGATE                6.1.1910 CORE                   43
OR                    6.1.1980 CORE                   43
OVER                  6.1.1990 CORE                   43
POSTPONE              6.1.2033 CORE                   43
QUIT                  6.1.2050 CORE                   44
R>                    6.1.2060 CORE                   44
R@                    6.1.2070 CORE                   44
RECURSE               6.1.2120 CORE                   44
REPEAT                6.1.2140 CORE                   45
ROT                   6.1.2160 CORE                   45
RSHIFT                6.1.2162 CORE                   45
S"                    6.1.2165 CORE                   45
S>D                   6.1.2170 CORE                   45
SIGN                  6.1.2210 CORE                   46
SM/REM                6.1.2214 CORE                   46
SOURCE                6.1.2216 CORE                   46
SPACE                 6.1.2220 CORE                   46
SPACES                6.1.2230 CORE                   46
STATE                 6.1.2250 CORE                   46
SWAP                  6.1.2260 CORE                   46
THEN                  6.1.2270 CORE                   47
TYPE                  6.1.2310 CORE                   47
U.                    6.1.2320 CORE                   47
U<                    6.1.2340 CORE                   47
UM*                   6.1.2360 CORE                   47
UM/MOD                6.1.2370 CORE                   48
UNLOOP                6.1.2380 CORE                   48
UNTIL                 6.1.2390 CORE                   48
VARIABLE              6.1.2410 CORE                   48
WHILE                 6.1.2430 CORE                   49
WORD                  6.1.2450 CORE                   49
XOR                   6.1.2490 CORE                   49
[                     6.1.2500 CORE                   49
[']                   6.1.2510 CORE                   50
[CHAR]                6.1.2520 CORE                   50
]                     6.1.2540 CORE                   50
#TIB                  6.2.0060 CORE EXT               51
.(                    6.2.0200 CORE EXT               51
.R                    6.2.0210 CORE EXT               51
0<>                   6.2.0260 CORE EXT               51
0>                    6.2.0280 CORE EXT               51
2>R                   6.2.0340 CORE EXT               51
2R>                   6.2.0410 CORE EXT               52
2R@                   6.2.0415 CORE EXT               52
:NONAME               6.2.0455 CORE EXT               52
<>                    6.2.0500 CORE EXT               52
?DO                   6.2.0620 CORE EXT               53
AGAIN                 6.2.0700 CORE EXT               53
C"                    6.2.0855 CORE EXT               53
CASE                  6.2.0873 CORE EXT               54
COMPILE,              6.2.0945 CORE EXT               54
CONVERT               6.2.0970 CORE EXT               54
ENDCASE               6.2.1342 CORE EXT               54
ENDOF                 6.2.1343 CORE EXT               55
ERASE                 6.2.1350 CORE EXT               55
EXPECT                6.2.1390 CORE EXT               55
FALSE                 6.2.1485 CORE EXT               55
HEX                   6.2.1660 CORE EXT               55
MARKER                6.2.1850 CORE EXT               56
NIP                   6.2.1930 CORE EXT               56
OF                    6.2.1950 CORE EXT               56
PAD                   6.2.2000 CORE EXT               56
PARSE                 6.2.2008 CORE EXT               57
PICK                  6.2.2030 CORE EXT               57
QUERY                 6.2.2040 CORE EXT               57
REFILL                6.2.2125 CORE EXT               57
RESTORE-INPUT         6.2.2148 CORE EXT               57
ROLL                  6.2.2150 CORE EXT               58
SAVE-INPUT            6.2.2182 CORE EXT               58
SOURCE-ID             6.2.2218 CORE EXT               58
SPAN                  6.2.2240 CORE EXT               58
TIB                   6.2.2290 CORE EXT               58
TO                    6.2.2295 CORE EXT               59
TRUE                  6.2.2298 CORE EXT               59
TUCK                  6.2.2300 CORE EXT               59
U.R                   6.2.2330 CORE EXT               59
U>                    6.2.2350 CORE EXT               59
UNUSED                6.2.2395 CORE EXT               59
VALUE                 6.2.2405 CORE EXT               60
WITHIN                6.2.2440 CORE EXT               60
[COMPILE]             6.2.2530 CORE EXT               60
\                     6.2.2535 CORE EXT               60
BLK                 7.6.1.0790 BLOCK                  63
BLOCK               7.6.1.0800 BLOCK                  63
BUFFER              7.6.1.0820 BLOCK                  64
EVALUATE            7.6.1.1360 BLOCK                  64
FLUSH               7.6.1.1559 BLOCK                  64
LOAD                7.6.1.1790 BLOCK                  64
SAVE-BUFFERS        7.6.1.2180 BLOCK                  64
UPDATE              7.6.1.2400 BLOCK                  65
EMPTY-BUFFERS       7.6.2.1330 BLOCK EXT              65
LIST                7.6.2.1770 BLOCK EXT              65
REFILL              7.6.2.2125 BLOCK EXT              65
SCR                 7.6.2.2190 BLOCK EXT              65
THRU                7.6.2.2280 BLOCK EXT              65
\                   7.6.2.2535 BLOCK EXT              66
2CONSTANT           8.6.1.0360 DOUBLE                 68
2LITERAL            8.6.1.0390 DOUBLE                 68
2VARIABLE           8.6.1.0440 DOUBLE                 69
D+                  8.6.1.1040 DOUBLE                 69
D-                  8.6.1.1050 DOUBLE                 69
D.                  8.6.1.1060 DOUBLE                 69
D.R                 8.6.1.1070 DOUBLE                 69
D0<                 8.6.1.1075 DOUBLE                 69
D0=                 8.6.1.1080 DOUBLE                 69
D2*                 8.6.1.1090 DOUBLE                 70
D2/                 8.6.1.1100 DOUBLE                 70
D<                  8.6.1.1110 DOUBLE                 70
D=                  8.6.1.1120 DOUBLE                 70
D>S                 8.6.1.1140 DOUBLE                 70
DABS                8.6.1.1160 DOUBLE                 70
DMAX                8.6.1.1210 DOUBLE                 70
DMIN                8.6.1.1220 DOUBLE                 70
DNEGATE             8.6.1.1230 DOUBLE                 71
M*/                 8.6.1.1820 DOUBLE                 71
M+                  8.6.1.1830 DOUBLE                 71
2ROT                8.6.2.0420 DOUBLE EXT             71
DU<                 8.6.2.1270 DOUBLE EXT             71
CATCH               9.6.1.0875 EXCEPTION              74
THROW               9.6.1.2275 EXCEPTION              75
ABORT               9.6.2.0670 EXCEPTION EXT          75
ABORT"              9.6.2.0680 EXCEPTION EXT          75
AT-XY              10.6.1.0742 FACILITY               77
KEY?               10.6.1.1755 FACILITY               77
PAGE               10.6.1.2005 FACILITY               77
EKEY               10.6.2.1305 FACILITY EXT           78
EKEY>CHAR          10.6.2.1306 FACILITY EXT           78
EKEY?              10.6.2.1307 FACILITY EXT           78
EMIT?              10.6.2.1325 FACILITY EXT           78
MS                 10.6.2.1905 FACILITY EXT           78
TIME&DATE          10.6.2.2292 FACILITY EXT           78
(                  11.6.1.0080 FILE                   82
BIN                11.6.1.0765 FILE                   82
CLOSE-FILE         11.6.1.0900 FILE                   82
CREATE-FILE        11.6.1.1010 FILE                   83
DELETE-FILE        11.6.1.1190 FILE                   83
FILE-POSITION      11.6.1.1520 FILE                   83
FILE-SIZE          11.6.1.1522 FILE                   83
INCLUDE-FILE       11.6.1.1717 FILE                   83
INCLUDED           11.6.1.1718 FILE                   84
OPEN-FILE          11.6.1.1970 FILE                   84
R/O                11.6.1.2054 FILE                   84
R/W                11.6.1.2056 FILE                   85
READ-FILE          11.6.1.2080 FILE                   85
READ-LINE          11.6.1.2090 FILE                   86
REPOSITION-FILE    11.6.1.2142 FILE                   86
RESIZE-FILE        11.6.1.2147 FILE                   86
S"                 11.6.1.2165 FILE                   87
SOURCE-ID          11.6.1.2218 FILE                   87
W/O                11.6.1.2425 FILE                   87
WRITE-FILE         11.6.1.2480 FILE                   87
WRITE-LINE         11.6.1.2485 FILE                   88
FILE-STATUS        11.6.2.1524 FILE EXT               88
FLUSH-FILE         11.6.2.1560 FILE EXT               88
REFILL             11.6.2.2125 FILE EXT               88
RENAME-FILE        11.6.2.2130 FILE EXT               88
>FLOAT             12.6.1.0558 FLOATING               93
D>F                12.6.1.1130 FLOATING               93
F!                 12.6.1.1400 FLOATING               93
F*                 12.6.1.1410 FLOATING               93
F+                 12.6.1.1420 FLOATING               93
F-                 12.6.1.1425 FLOATING               93
F/                 12.6.1.1430 FLOATING               94
F0<                12.6.1.1440 FLOATING               94
F0=                12.6.1.1450 FLOATING               94
F<                 12.6.1.1460 FLOATING               94
F>D                12.6.1.1470 FLOATING               94
F@                 12.6.1.1472 FLOATING               94
FALIGN             12.6.1.1479 FLOATING               94
FALIGNED           12.6.1.1483 FLOATING               94
FCONSTANT          12.6.1.1492 FLOATING               95
FDEPTH             12.6.1.1497 FLOATING               95
FDROP              12.6.1.1500 FLOATING               95
FDUP               12.6.1.1510 FLOATING               95
FLITERAL           12.6.1.1552 FLOATING               95
FLOAT+             12.6.1.1555 FLOATING               95
FLOATS             12.6.1.1556 FLOATING               96
FLOOR              12.6.1.1558 FLOATING               96
FMAX               12.6.1.1562 FLOATING               96
FMIN               12.6.1.1565 FLOATING               96
FNEGATE            12.6.1.1567 FLOATING               96
FOVER              12.6.1.1600 FLOATING               96
FROT               12.6.1.1610 FLOATING               96
FROUND             12.6.1.1612 FLOATING               96
FSWAP              12.6.1.1620 FLOATING               96
FVARIABLE          12.6.1.1630 FLOATING               97
REPRESENT          12.6.1.2143 FLOATING               97
DF!                12.6.2.1203 FLOATING EXT           97
DF@                12.6.2.1204 FLOATING EXT           98
DFALIGN            12.6.2.1205 FLOATING EXT           98
DFALIGNED          12.6.2.1207 FLOATING EXT           98
DFLOAT+            12.6.2.1208 FLOATING EXT           98
DFLOATS            12.6.2.1209 FLOATING EXT           98
F**                12.6.2.1415 FLOATING EXT           98
F.                 12.6.2.1427 FLOATING EXT           99
FABS               12.6.2.1474 FLOATING EXT           99
FACOS              12.6.2.1476 FLOATING EXT           99
FACOSH             12.6.2.1477 FLOATING EXT           99
FALOG              12.6.2.1484 FLOATING EXT           99
FASIN              12.6.2.1486 FLOATING EXT           99
FASINH             12.6.2.1487 FLOATING EXT           99
FATAN              12.6.2.1488 FLOATING EXT          100
FATAN2             12.6.2.1489 FLOATING EXT          100
FATANH             12.6.2.1491 FLOATING EXT          100
FCOS               12.6.2.1493 FLOATING EXT          100
FCOSH              12.6.2.1494 FLOATING EXT          100
FE.                12.6.2.1513 FLOATING EXT          100
FEXP               12.6.2.1515 FLOATING EXT          100
FEXPM1             12.6.2.1516 FLOATING EXT          101
FLN                12.6.2.1553 FLOATING EXT          101
FLNP1              12.6.2.1554 FLOATING EXT          101
FLOG               12.6.2.1557 FLOATING EXT          101
FS.                12.6.2.1613 FLOATING EXT          101
FSIN               12.6.2.1614 FLOATING EXT          101
FSINCOS            12.6.2.1616 FLOATING EXT          102
FSINH              12.6.2.1617 FLOATING EXT          102
FSQRT              12.6.2.1618 FLOATING EXT          102
FTAN               12.6.2.1625 FLOATING EXT          102
FTANH              12.6.2.1626 FLOATING EXT          102
F~                 12.6.2.1640 FLOATING EXT          102
PRECISION          12.6.2.2035 FLOATING EXT          102
SET-PRECISION      12.6.2.2200 FLOATING EXT          102
SF!                12.6.2.2202 FLOATING EXT          103
SF@                12.6.2.2203 FLOATING EXT          103
SFALIGN            12.6.2.2204 FLOATING EXT          103
SFALIGNED          12.6.2.2206 FLOATING EXT          103
SFLOAT+            12.6.2.2207 FLOATING EXT          103
SFLOATS            12.6.2.2208 FLOATING EXT          104
(LOCAL)            13.6.1.0086 LOCAL                 108
TO                 13.6.1.2295 LOCAL                 109
LOCALS|            13.6.2.1795 LOCAL EXT             109
ALLOCATE           14.6.1.0707 MEMORY                111
FREE               14.6.1.1605 MEMORY                112
RESIZE             14.6.1.2145 MEMORY                112
.S                 15.6.1.0220 TOOLS                 115
?                  15.6.1.0600 TOOLS                 115
DUMP               15.6.1.1280 TOOLS                 115
SEE                15.6.1.2194 TOOLS                 115
WORDS              15.6.1.2465 TOOLS                 116
;CODE              15.6.2.0470 TOOLS EXT             116
AHEAD              15.6.2.0702 TOOLS EXT             116
ASSEMBLER          15.6.2.0740 TOOLS EXT             117
BYE                15.6.2.0830 TOOLS EXT             117
CODE               15.6.2.0930 TOOLS EXT             117
CS-PICK            15.6.2.1015 TOOLS EXT             117
CS-ROLL            15.6.2.1020 TOOLS EXT             118
EDITOR             15.6.2.1300 TOOLS EXT             118
FORGET             15.6.2.1580 TOOLS EXT             118
STATE              15.6.2.2250 TOOLS EXT             118
[ELSE]             15.6.2.2531 TOOLS EXT             119
[IF]               15.6.2.2532 TOOLS EXT             119
[THEN]             15.6.2.2533 TOOLS EXT             119
DEFINITIONS        16.6.1.1180 SEARCH                122
FIND               16.6.1.1550 SEARCH                122
FORTH-WORDLIST     16.6.1.1595 SEARCH                122
GET-CURRENT        16.6.1.1643 SEARCH                122
GET-ORDER          16.6.1.1647 SEARCH                122
SEARCH-WORDLIST    16.6.1.2192 SEARCH                123
SET-CURRENT        16.6.1.2195 SEARCH                123
SET-ORDER          16.6.1.2197 SEARCH                123
WORDLIST           16.6.1.2460 SEARCH                123
ALSO               16.6.2.0715 SEARCH EXT            123
FORTH              16.6.2.1590 SEARCH EXT            123
ONLY               16.6.2.1965 SEARCH EXT            124
ORDER              16.6.2.1985 SEARCH EXT            124
PREVIOUS           16.6.2.2037 SEARCH EXT            124
-TRAILING          17.6.1.0170 STRING                126
/STRING            17.6.1.0245 STRING                126
BLANK              17.6.1.0780 STRING                126
CMOVE              17.6.1.0910 STRING                126
CMOVE>             17.6.1.0920 STRING                126
COMPARE            17.6.1.0935 STRING                127
SEARCH             17.6.1.2191 STRING                127
SLITERAL           17.6.1.2212 STRING                127
#endif
