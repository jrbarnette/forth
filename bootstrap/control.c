/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <stddef.h>

#include "forth.h"

/*
 * control.c - Standard Forth words relating to flow of control in
 *   compiled defintions.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// +LOOP                 6.1.0140 CORE                   27
// BEGIN                 6.1.0760 CORE                   34
// DO                    6.1.1240 CORE                   36
// ELSE                  6.1.1310 CORE                   37
// I                     6.1.1680 CORE                   39
// IF                    6.1.1700 CORE                   40
// J                     6.1.1730 CORE                   40
// LEAVE                 6.1.1760 CORE                   41
// REPEAT                6.1.2140 CORE                   44
// THEN                  6.1.2270 CORE                   46
// UNLOOP                6.1.2380 CORE                   47
// UNTIL                 6.1.2390 CORE                   47
// WHILE                 6.1.2430 CORE                   47
//------  ------  ------  ------  ------  ------  ------  ------


static vminstr_p
compile_skip(vmstate_p vm, xt_ft skip)
{
    COMPILE(vm, skip);
    return (vminstr_p) allot(vm, CELL_SIZE);
}


static void
patch(vminstr_p orig, vminstr_p dest)
{
    orig->offset = dest - orig;
}


static vminstr_p leavers;

static void
compile_plus_loop(vmstate_p vm, xt_ft unloop_xt)
{
    CHECK_POP(vm, 1);
    COMPILE(vm, PLUS_LOOP_XT);
    vminstr_p dest = (vminstr_p)POP(vm);
    patch(compile_skip(vm, FSKIP_XT), dest);

    vminstr_p oleavers = (vminstr_p)POP(vm);
    vminstr_p orig = leavers;
    leavers = oleavers;
    while (orig != NULL) {
	vminstr_p new_orig = *(vminstr_p *)orig;
	patch(orig, (vminstr_p)HERE);
	orig = new_orig;
    }
    COMPILE(vm, unloop_xt);
}


static vminstr_p
do_skip(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    return ip + ip->offset;
}


static vminstr_p
do_fskip(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    if (POP(vm) == 0)
	return ip + ip->offset;
    else
	return ip + 1;
}


/* ( n -- ) ( R: loop-sys1 -- | loop-sys2 ) runtime semantics */
static vminstr_p
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


/* ( C: do-sys -- ) compilation semantics */
static vminstr_p
x_plus_loop(vminstr_p ip, vmstate_p vm, vmarg_p unloop_xt_ptr)
{
    compile_plus_loop(vm, unloop_xt_ptr->xtok);
    return ip;
}


/* ( x1 x2 -- ) ( R: -- loop-sys ) runtime semantics */
static vminstr_p
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


/* ( C: -- do-sys ) compilation semantics */
static vminstr_p
x_do(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);

    COMPILE(vm, DO_DO_XT);
    PUSH(vm, leavers);
    leavers = 0;
    PUSH(vm, HERE);
    return ip;
}


/* ( -- x ) ( R: loop-sys -- loop-sys ) execution semantics */
static vminstr_p
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
static vminstr_p
x_j(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_RPOP(vm, 4);
    CHECK_PUSH(vm, 1);
    cell_ft *rsp = RSP(vm);
    PUSH(vm, PICK(rsp, 2) + PICK(rsp, 3));
    return ip;
}


/* ( C: do-sys1 -- do-sys2 ) compilation semantics */
static vminstr_p
c_leave(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 2);
    vminstr_p orig = compile_skip(vm, SKIP_XT);
    *(vminstr_p *)orig = leavers;
    leavers = orig;
    return ip;
}


/* ( -- ) ( R: loop-sys -- ) execution semantics */
static vminstr_p
x_unloop(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_RPOP(vm, 2);
    SET_RSP(vm, RSP(vm), 2);
    return ip;
}


/* ( C: -- dest ) compilation semantics */
static vminstr_p
x_begin(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) HERE);
    return ip;
}


/* ( C: orig1 -- orig2 ) compilation semantics */
static vminstr_p
x_else(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    vminstr_p orig1;
    vminstr_p orig2;

    CHECK_POP(vm, 1);
    orig1 = (vminstr_p)POP(vm);
    orig2 = compile_skip(vm, SKIP_XT);
    patch(orig1, (vminstr_p)HERE);
    PUSH(vm, (cell_ft)orig2);
    return ip;
}


/* ( C: -- orig ) compilation semantics */
static vminstr_p
x_if(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) compile_skip(vm, FSKIP_XT));
    return ip;
}


/* ( C: orig dest -- ) compilation semantics */
static vminstr_p
x_repeat(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    vminstr_p dest;
    vminstr_p orig;

    CHECK_POP(vm, 2);
    dest = (vminstr_p) POP(vm);
    orig = (vminstr_p) POP(vm);
    patch(compile_skip(vm, SKIP_XT), dest);
    patch(orig, (vminstr_p) HERE);
    return ip;
}


/* ( C: orig -- ) compilation semantics */
static vminstr_p
x_then(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    patch((vminstr_p) POP(vm), (vminstr_p) HERE);
    return ip;
}


/* ( C: dest -- ) compilation semantics */
static vminstr_p
x_until(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    vminstr_p dest;

    CHECK_POP(vm, 1);
    dest = (vminstr_p) POP(vm);
    patch(compile_skip(vm, FSKIP_XT), dest);
    return ip;
}


/* ( C: dest -- orig dest ) compilation semantics */
static vminstr_p
x_while(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft dest;

    CHECK_PUSH(vm, 1);
    CHECK_POP(vm, 1);
    dest = POP(vm);
    PUSH(vm, (cell_ft) compile_skip(vm, FSKIP_XT));
    PUSH(vm, dest);
    return ip;
}


DIRECT_FORTH(init_control) // {
    L(do_do)         L(DO_DO_XT)      X(x_store)
    L(do_plus_loop)  L(PLUS_LOOP_XT)  X(x_store)
    L(do_skip)       L(SKIP_XT)       X(x_store)
    L(do_fskip)      L(FSKIP_XT)      X(x_store)

    PRIM("UNLOOP",	x_unloop)	FLAGS(NO_INTERPRET)

    PRIM("+LOOP",	x_plus_loop)	FLAGS(COMPILE)
    XCOMPILE("UNLOOP")

    PRIM("BEGIN",	x_begin)	FLAGS(COMPILE)
    PRIM("DO",		x_do)		FLAGS(COMPILE)
    PRIM("ELSE",	x_else)		FLAGS(COMPILE)
    PRIM("I",		x_i)		FLAGS(NO_INTERPRET)
    PRIM("IF",		x_if)		FLAGS(COMPILE)
    PRIM("J",		x_j)		FLAGS(NO_INTERPRET)
    PRIM("LEAVE",	c_leave)	FLAGS(COMPILE)

    PRIM("REPEAT",	x_repeat)	FLAGS(COMPILE)
    PRIM("THEN",	x_then)		FLAGS(COMPILE)
    PRIM("UNTIL",	x_until)	FLAGS(COMPILE)
    PRIM("WHILE",	x_while)	FLAGS(COMPILE)
END_DIRECT // }
