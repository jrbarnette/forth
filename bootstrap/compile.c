/*
 * Copyright 2008, by J. Richard Barnette
 */

#include <stddef.h>

#include "forth.h"

/*
 * compile.c - Implementation of Forth standard words relating
 *   to compilation.  Includes management of things related to
 *   "code space" as defined in the Forth standard.
 */


/* EXIT ( R: nest-sys -- ) no interpretation semantics */
static cell_ft
x_exit(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_RPOP(vm, 1);
    vm->ip = (xt_ft *) RPOP(vm);
    return tos;
}

/* ( R: -- nest-sys ) initiation semantics */
static cell_ft
do_colon(cell_ft tos, vmstate_p vm, addr_ft newip)
{
    CHECK_RPUSH(vm, 1);
    RPUSH(vm, (cell_ft) vm->ip);
    vm->ip = (xt_ft *) newip;
    return tos;
}

/* : ( C: "<spaces> name" -- colon-sys ) compile state only */
static cell_ft
x_colon(cell_ft tos, vmstate_p vm, addr_ft colon_xt)
{
    /* XXX : - enforce compilation state */
    /* XXX : - parse name, start definition */
    ((xt_ft) allot(CELL_SIZE))->handler = do_colon;
    return tos;
}

/* C equivalent to :NONAME */
xt_ft
noname(void)
{
    xt_ft	xt = (xt_ft) allot(CELL_SIZE);

    xt->handler = do_colon;
    return xt;
}

/* LITERAL ( x -- ) compile state only */
static cell_ft
x_literal(cell_ft tos, vmstate_p vm, addr_ft do_lit_xt)
{
    /* XXX LITERAL - enforce compilation state */
    CHECK_POP(vm, 1);
    *(a_addr_ft) allot(CELL_SIZE) = (cell_ft) do_lit_xt;
    *(a_addr_ft) allot(CELL_SIZE) = tos;
    return POP(vm);
}

/* ( -- x ) runtime semantics */
static cell_ft
do_literal(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, tos);
    return *(a_addr_ft) vm->ip++;
}

static void
def_literal(name_p lit, void *ignore)
{
    lit->len |= NAME_IMMEDIATE;
    ((xt_ft) allot(CELL_SIZE))->handler = do_literal;
}

#if 0
    +LOOP                 6.1.0140 CORE                   27
    :                     6.1.0450 CORE                   30
    ;                     6.1.0460 CORE                   30
    >BODY                 6.1.0550 CORE                   31
    BEGIN                 6.1.0760 CORE                   34
    DO                    6.1.1240 CORE                   37
    ELSE                  6.1.1310 CORE                   38
    EXIT                  6.1.1380 CORE                   39
    I                     6.1.1680 CORE                   40
    IF                    6.1.1700 CORE                   40
    J                     6.1.1730 CORE                   41
    LEAVE                 6.1.1760 CORE                   41
    LITERAL               6.1.1780 CORE                   42
    LOOP                  6.1.1800 CORE                   42
    POSTPONE              6.1.2033 CORE                   43
    RECURSE               6.1.2120 CORE                   44
    REPEAT                6.1.2140 CORE                   45
    S"                    6.1.2165 CORE                   45
    STATE                 6.1.2250 CORE                   46
    THEN                  6.1.2270 CORE                   47
    UNLOOP                6.1.2380 CORE                   48
    UNTIL                 6.1.2390 CORE                   48
    WHILE                 6.1.2430 CORE                   49
    [                     6.1.2500 CORE                   49
    [']                   6.1.2510 CORE                   50
    ]                     6.1.2540 CORE                   50
    :NONAME               6.2.0455 CORE EXT               52
    ?DO                   6.2.0620 CORE EXT               53
    AGAIN                 6.2.0700 CORE EXT               53
    C"                    6.2.0855 CORE EXT               53
    CASE                  6.2.0873 CORE EXT               54
    COMPILE,              6.2.0945 CORE EXT               54
    ENDCASE               6.2.1342 CORE EXT               54
    ENDOF                 6.2.1343 CORE EXT               55
    [COMPILE]             6.2.2530 CORE EXT               60
#endif

defn_dt
compile_defns[] = {
    { "LITERAL", x_literal, def_literal, NULL },
    { "EXIT",    x_exit },
    { NULL }
};
