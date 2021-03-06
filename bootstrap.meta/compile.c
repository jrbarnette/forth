/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include "forth.h"

/*
 * compile.c - Standard Forth words and machine code primitives
 *   needed for compiling definitions.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// EXECUTE               6.1.1370 CORE                   38
// EXIT                  6.1.1380 CORE                   38
// LITERAL               6.1.1780 CORE                   41
// POSTPONE              6.1.2033 CORE                   43
// S"                    6.1.2165 CORE                   44
// STATE                 6.1.2250 CORE                   45
// [                     6.1.2500 CORE                   48
// [']                   6.1.2510 CORE                   48
// [CHAR]                6.1.2520 CORE                   49
// ]                     6.1.2540 CORE                   49
// C"                    6.2.0855 CORE EXT               52
// COMPILE,              6.2.0945 CORE EXT               52
// [COMPILE]             6.2.2530 CORE EXT               58
//------  ------  ------  ------  ------  ------  ------  ------


/* ( i*x xt -- i*j ) */
vminstr_p
x_execute(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    xt_ft xtok = (xt_ft) POP(vm);
    return xtok->handler(ip, vm, xtok[1].arg);
}


/* ( R: -- nest-sys ) initiation semantics */
vminstr_p
do_colon(vminstr_p ip, vmstate_p vm, vmarg_p newip)
{
    CHECK_RPUSH(vm, 1);
    RPUSH(vm, (cell_ft) ip);
    return newip->vminstrs;
}


/* ( R: nest-sys -- ) execution semantics */
vminstr_p
x_exit(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_RPOP(vm, 1);
    return (vminstr_p)RPOP(vm);
}


/* ( -- x ) runtime semantics */
vminstr_p
do_literal(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, ip->cell);
    return ip + 1;
}


/* ( -- a-addr ) name execution semantics */
vminstr_p
do_variable(vminstr_p ip, vmstate_p vm, vmarg_p var_addr)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) var_addr);
    return ip;
}


/* ( -- x ) name execution semantics */
vminstr_p
do_constant(vminstr_p ip, vmstate_p vm, vmarg_p data_ptr)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, data_ptr->cell);
    return ip;
}


DIRECT_FORTH(init_compile_prim) // {
    DL(do_literal) DL(&DO_LITERAL->handler) X(x_store)
    PRIM("EXECUTE", x_execute)
    PRIM("EXIT", x_exit) FLAGS(NO_INTERPRET)
    PRIM("STATE", do_variable) XCOMMA(F_TRUE)
    PRIM("[", do_colon)
	META L(F_FALSE) STATE STORE EXIT DIRECT
    FLAGS(IMMEDIATE)
    PRIM("]", do_colon)
	META L(F_TRUE) STATE STORE XSEMICOLON DIRECT
END_DIRECT // }


META_FORTH(init_compile_ops) // {
    XCOLON("LITERAL") L(DO_LITERAL) COMMA COMMA XSEMICOLON COMPILE_ONLY
END_META // }

// COMPILE,              6.2.0945 CORE EXT               52

// These rely on PARSE
// POSTPONE              6.1.2033 CORE                   43
// S"                    6.1.2165 CORE                   44
// [']                   6.1.2510 CORE                   48
// [CHAR]                6.1.2520 CORE                   49
// C"                    6.2.0855 CORE EXT               52
// [COMPILE]             6.2.2530 CORE EXT               58
