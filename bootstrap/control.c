/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <stddef.h>

#include "forth.h"

/*
 * control.c - C primitives to implement run-time behavior of Forth
 *   words relating to flow of control in compiled defintions.
 */

/* Contains all semantics for the following standard definitions:
 *------  ------  ------  ------  ------  ------  ------  ------
 * I                     6.1.1680 CORE                   39
 * J                     6.1.1730 CORE                   40
 * UNLOOP                6.1.2380 CORE                   47
 *------  ------  ------  ------  ------  ------  ------  ------
 */

/* Contains runtime or other partial semantics for the following:
 *------  ------  ------  ------  ------  ------  ------  ------
 * +LOOP                 6.1.0140 CORE                   27
 * BEGIN                 6.1.0760 CORE                   34
 * DO                    6.1.1240 CORE                   36
 * ELSE                  6.1.1310 CORE                   37
 * IF                    6.1.1700 CORE                   40
 * LEAVE                 6.1.1760 CORE                   41
 * LOOP                  6.1.1800 CORE                   41
 * REPEAT                6.1.2140 CORE                   44
 * THEN                  6.1.2270 CORE                   46
 * UNTIL                 6.1.2390 CORE                   47
 * WHILE                 6.1.2430 CORE                   47
 *------  ------  ------  ------  ------  ------  ------  ------
 */


PRIM_HDLR(do_skip)
{
    return ip + ip->offset;
}


PRIM_HDLR(do_fskip)
{
    CHECK_POP(vm, 1);
    if (POP(vm) == 0)
	return ip + ip->offset;
    else
	return ip + 1;
}


/* ( n -- ) ( R: loop-sys1 -- | loop-sys2 ) runtime semantics */
PRIM_HDLR(do_plus_loop)
{
    CHECK_POP(vm, 1);
    CHECK_RPOP(vm, 2);
    cell_ft *rsp = RSP(vm);
    cell_ft *sp = SP(vm);
    cell_ft index = PICK(rsp, 0);
    cell_ft newindex = index + PICK(sp, 0);
    PICK(rsp, 0) = newindex;
    PICK(sp, 0) = -((snumber_ft)(newindex ^ index) < 0);
    return ip;
}


/* ( x1 x2 -- ) ( R: -- loop-sys ) runtime semantics */
PRIM_HDLR(do_do)
{
    cell_ft *sp = SP(vm);
    cell_ft *rsp = RSP(vm);

    CHECK_POP(vm, 2);
    CHECK_RPUSH(vm, 2);
    cell_ft index = PICK(sp, 0);
    cell_ft limit = PICK(sp, 1);
    SET_RSP(vm, rsp, -2);
    PICK(rsp, -1) = limit;
    PICK(rsp, -2) = index-limit;
    SET_SP(vm, sp, 2);
    return ip;
}


/* ( -- x ) ( R: loop-sys -- loop-sys ) execution semantics */
PRIM_HDLR(x_i)
{
    CHECK_RPOP(vm, 2);
    CHECK_PUSH(vm, 1);
    cell_ft *rsp = RSP(vm);
    PUSH(vm, PICK(rsp, 0) + PICK(rsp, 1));
    return ip;
}


/* ( -- x ) ( R: loop-sys1 loop-sys2 -- loop-sys1 loop-sys2 ) */
/*          execution semantics */
PRIM_HDLR(x_j)
{
    CHECK_RPOP(vm, 4);
    CHECK_PUSH(vm, 1);
    cell_ft *rsp = RSP(vm);
    PUSH(vm, PICK(rsp, 2) + PICK(rsp, 3));
    return ip;
}


/* ( -- ) ( R: loop-sys -- ) execution semantics */
PRIM_HDLR(x_unloop)
{
    CHECK_RPOP(vm, 2);
    SET_RSP(vm, RSP(vm), 2);
    return ip;
}
