/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <stddef.h>

#include "forth.h"
#include "direct.h"

/*
 * control.c - C primitives to implement run-time behavior of Forth
 *   words relating to flow of control in compiled defintions.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// I                     6.1.1680 CORE                   39
// J                     6.1.1730 CORE                   40
// UNLOOP                6.1.2380 CORE                   47
//------  ------  ------  ------  ------  ------  ------  ------


vminstr_p
do_skip(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    return ip + ip->offset;
}


vminstr_p
do_fskip(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    if (POP(vm) == 0)
	return ip + ip->offset;
    else
	return ip + 1;
}


/* ( n -- ) ( R: loop-sys1 -- | loop-sys2 ) runtime semantics */
vminstr_p
do_plus_loop(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
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
vminstr_p
do_do(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
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
vminstr_p
x_i(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_RPOP(vm, 2);
    CHECK_PUSH(vm, 1);
    cell_ft *rsp = RSP(vm);
    PUSH(vm, PICK(rsp, 0) + PICK(rsp, 1));
    return ip;
}


/* ( -- x ) ( R: loop-sys1 loop-sys2 -- loop-sys1 loop-sys2 ) */
/*          execution semantics */
vminstr_p
x_j(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_RPOP(vm, 4);
    CHECK_PUSH(vm, 1);
    cell_ft *rsp = RSP(vm);
    PUSH(vm, PICK(rsp, 2) + PICK(rsp, 3));
    return ip;
}


/* ( -- ) ( R: loop-sys -- ) execution semantics */
vminstr_p
x_unloop(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_RPOP(vm, 2);
    SET_RSP(vm, RSP(vm), 2);
    return ip;
}
