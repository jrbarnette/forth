/*
 * Copyright 2008, by J. Richard Barnette
 */

#include "forth.h"

/*
 * primitives.c - Standard Forth words for arithmetic, logical,
 *   memory fetch/store, and stack manipulation operations.
 */


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

#if 0
    !                     6.1.0010 CORE                   25
    *                     6.1.0090 CORE                   26
    */                    6.1.0100 CORE                   26
    */MOD                 6.1.0110 CORE                   26
    +                     6.1.0120 CORE                   26
    +!                    6.1.0130 CORE                   27
    -                     6.1.0160 CORE                   27
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
    <                     6.1.0480 CORE                   30
    =                     6.1.0530 CORE                   31
    >                     6.1.0540 CORE                   31
    >R                    6.1.0580 CORE                   32
    ?DUP                  6.1.0630 CORE                   32
    @                     6.1.0650 CORE                   32
    ABS                   6.1.0690 CORE                   32
    ALIGNED               6.1.0706 CORE                   33
    AND                   6.1.0720 CORE                   33
    C!                    6.1.0850 CORE                   34
    C@                    6.1.0870 CORE                   34
    CELL+                 6.1.0880 CORE                   35
    CELLS                 6.1.0890 CORE                   35
    CHAR+                 6.1.0897 CORE                   35
    CHARS                 6.1.0898 CORE                   35
    DEPTH                 6.1.1200 CORE                   36
    DROP                  6.1.1260 CORE                   37
    DUP                   6.1.1290 CORE                   38
    FILL                  6.1.1540 CORE                   39
    FM/MOD                6.1.1561 CORE                   40
    INVERT                6.1.1720 CORE                   41
    LSHIFT                6.1.1805 CORE                   42
    M*                    6.1.1810 CORE                   42
    MAX                   6.1.1870 CORE                   42
    MIN                   6.1.1880 CORE                   42
    MOD                   6.1.1890 CORE                   43
    MOVE                  6.1.1900 CORE                   43
    NEGATE                6.1.1910 CORE                   43
    OR                    6.1.1980 CORE                   43
    OVER                  6.1.1990 CORE                   43
    R>                    6.1.2060 CORE                   44
    R@                    6.1.2070 CORE                   44
    ROT                   6.1.2160 CORE                   45
    RSHIFT                6.1.2162 CORE                   45
    S>D                   6.1.2170 CORE                   45
    SIGN                  6.1.2210 CORE                   46
    SM/REM                6.1.2214 CORE                   46
    SWAP                  6.1.2260 CORE                   46
    U<                    6.1.2340 CORE                   47
    UM*                   6.1.2360 CORE                   47
    UM/MOD                6.1.2370 CORE                   48
    XOR                   6.1.2490 CORE                   49
    0<>                   6.2.0260 CORE EXT               51
    0>                    6.2.0280 CORE EXT               51
    2>R                   6.2.0340 CORE EXT               51
    2R>                   6.2.0410 CORE EXT               52
    2R@                   6.2.0415 CORE EXT               52
    <>                    6.2.0500 CORE EXT               52
    ERASE                 6.2.1350 CORE EXT               55
    NIP                   6.2.1930 CORE EXT               56
    PICK                  6.2.2030 CORE EXT               57
    ROLL                  6.2.2150 CORE EXT               58
    TUCK                  6.2.2300 CORE EXT               59
    U>                    6.2.2350 CORE EXT               59
    WITHIN                6.2.2440 CORE EXT               60
#endif

defn_dt
primitive_defns[] = {
    { "!",     x_store },
    { "@",     x_fetch },
    { "+",     x_plus },
    { NULL }
};
