/*
 * Copyright 2011, by J. Richard Barnette
 */

#include <stddef.h>

#include "forth.h"

/*
 * control.c - Standard Forth words relating to flow of control in
 *   compiled defintions.
 */


static void
compile_plus_loop(vmstate_p vm, xt_ft unloop_xt)
{
    CHECK_POP(vm, 1);
    vminstr_p orig = (vminstr_p)POP(vm);
    vminstr_p dest = (vminstr_p)POP(vm);
    compile_xt(vm, PLUS_LOOP_XT);
    patch(compile_skip(vm, FSKIP_XT), dest);

    while (orig != NULL) {
	vminstr_p new_orig = *(vminstr_p *)orig;
	patch(orig, (vminstr_p)HERE);
	orig = new_orig;
    }
    compile_xt(vm, unloop_xt);
}


/* +LOOP                6.1.0140 CORE, p. 27 */
/* ( n -- ) ( R: loop-sys1 -- | loop-sys2 ) runtime semantics */
static vminstr_p
do_plus_loop(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    CHECK_RPOP(vm, 2);
    cell_ft *rsp = RSP(vm);
    cell_ft index = PICK(rsp, 0);
    cell_ft newindex = index + POP(vm);
    PICK(rsp, 0) = newindex;
    PUSH(vm, ((snumber_ft)(newindex ^ index) < 0));
    return ip;
}

/* ( C: do-sys -- ) compilation semantics */
static vminstr_p
x_plus_loop(vminstr_p ip, vmstate_p vm, vmarg_p unloop_xt_ptr)
{
    compile_plus_loop(vm, unloop_xt_ptr->xtok);
    return ip;
}


/* BEGIN		6.1.0760 CORE, p. 34 */
/* interpretation semantics undefined */
/* ( C: -- dest ) compilation semantics */
static vminstr_p
x_begin(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) HERE);
    return ip;
}


/* DO                   6.1.1240 CORE, p. 37 */
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

    compile_xt(vm, DO_DO_XT);
    PUSH(vm, HERE);
    PUSH(vm, 0);
    return ip;
}


/* ELSE			6.1.1310 CORE, p. 38 */
/* interpretation semantics undefined */
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


/* I                    6.1.1680 CORE, p. 40 */
/* interpretation semantics undefined */
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


/* IF			6.1.1700 CORE, p. 40 */
/* interpretation semantics undefined */
/* ( C: -- orig ) compilation semantics */
static vminstr_p
x_if(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) compile_skip(vm, FSKIP_XT));
    return ip;
}


/* J                    6.1.1730 CORE, p. 41 */
/* interpretation semantics undefined */
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


/* LEAVE                6.1.1760 CORE, p. 41 */
/* interpretation semantics undefined */
/* ( C: do-sys1 -- do-sys2 ) compilation semantics */
static vminstr_p
c_leave(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    // XXX - the standard doesn't say we can depend on do-sys here;
    // XXX - that would appear to be an oversight.
    CHECK_POP(vm, 2);
    vminstr_p orig1 = (vminstr_p)POP(vm);
    vminstr_p orig2 = compile_skip(vm, SKIP_XT);
    *(vminstr_p *)orig2 = orig1;
    PUSH(vm, orig2);
    return ip;
}


/* LOOP                 6.1.1800 CORE, p. 42 */
/* ( C: do-sys -- ) compilation semantics */
static vminstr_p
x_loop(vminstr_p ip, vmstate_p vm, vmarg_p unloop_xt_ptr)
{
    compile_literal(vm, 1);
    compile_plus_loop(vm, unloop_xt_ptr->xtok);
    return ip;
}


/* REPEAT		6.1.2140 CORE, p. 45 */
/* interpretation semantics undefined */
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


/* THEN			6.1.2270 CORE, p. 47 */
/* interpretation semantics undefined */
/* ( C: orig -- ) compilation semantics */
static vminstr_p
x_then(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_POP(vm, 1);
    patch((vminstr_p) POP(vm), (vminstr_p) HERE);
    return ip;
}


/* UNLOOP               6.1.2380 CORE, p. 48 */
/* interpretation semantics undefined */
/* ( -- ) ( R: loop-sys -- ) execution semantics */
static vminstr_p
x_unloop(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_RPOP(vm, 2);
    SET_RSP(vm, RSP(vm), 2);
    return ip;
}


/* UNTIL		6.1.2390 CORE, p. 48 */
/* interpretation semantics undefined */
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


/* WHILE		6.1.2430 CORE, p. 49 */
/* interpretation semantics undefined */
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


static void
initialize_do_tokens(vmstate_p vm, defn_data_p ignore)
{
    DICT.do_instr.handler = do_do;
    DICT.plus_loop_instr.handler = do_plus_loop;
}


defn_dt
control_defns[] =
{
    { initialize_do_tokens },
    { define_name, "UNLOOP",	x_unloop, NAME_TYPE_NO_INTERPRET },

    { define_name, "+LOOP",	x_plus_loop, NAME_TYPE_COMPILE },
    { compile_name, "UNLOOP" },

    { define_name, "BEGIN",	x_begin, NAME_TYPE_COMPILE },
    { define_name, "DO",	x_do, NAME_TYPE_COMPILE },
    { define_name, "ELSE",	x_else, NAME_TYPE_COMPILE },
    { define_name, "I",		x_i, NAME_TYPE_NO_INTERPRET },
    { define_name, "IF",	x_if, NAME_TYPE_COMPILE },
    { define_name, "J",		x_j, NAME_TYPE_NO_INTERPRET },
    { define_name, "LEAVE",	c_leave, NAME_TYPE_COMPILE },
    { define_name, "LOOP",	x_loop, NAME_TYPE_COMPILE },
    { compile_name, "UNLOOP" },

    { define_name, "REPEAT",	x_repeat, NAME_TYPE_COMPILE },
    { define_name, "THEN",	x_then, NAME_TYPE_COMPILE },
    { define_name, "UNTIL",	x_until, NAME_TYPE_COMPILE },
    { define_name, "WHILE",	x_while, NAME_TYPE_COMPILE },
    { NULL }
};

#if 0
    RECURSE               6.1.2120 CORE                   44
    ?DO                   6.2.0620 CORE EXT               53
    AGAIN                 6.2.0700 CORE EXT               53
    CASE                  6.2.0873 CORE EXT               54
    COMPILE,              6.2.0945 CORE EXT               54
    ENDCASE               6.2.1342 CORE EXT               54
    ENDOF                 6.2.1343 CORE EXT               55
    OF                    6.2.1950 CORE EXT               56
#endif
