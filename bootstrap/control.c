/*
 * Copyright 2011, by J. Richard Barnette
 */

#include <stddef.h>

#include "forth.h"

/*
 * control.c - Standard Forth words relating to flow of control in
 *   compiled defintions.
 */


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


defn_dt
control_defns[] =
{
    { define_name, "BEGIN",	x_begin, NAME_TYPE_COMPILE },

    { define_name, "IF",	x_if, NAME_TYPE_COMPILE },
    { define_name, "ELSE",	x_else, NAME_TYPE_COMPILE },
    { define_name, "REPEAT",	x_repeat, NAME_TYPE_COMPILE },
    { define_name, "THEN",	x_then, NAME_TYPE_COMPILE },
    { define_name, "UNTIL",	x_until, NAME_TYPE_COMPILE },
    { define_name, "WHILE",	x_while, NAME_TYPE_COMPILE },
    { NULL }
};

#if 0
    +LOOP                 6.1.0140 CORE                   27
    DO                    6.1.1240 CORE                   37
    I                     6.1.1680 CORE                   40
    J                     6.1.1730 CORE                   41
    LEAVE                 6.1.1760 CORE                   41
    LOOP                  6.1.1800 CORE                   42
    RECURSE               6.1.2120 CORE                   44
    UNLOOP                6.1.2380 CORE                   48
    ?DO                   6.2.0620 CORE EXT               53
    AGAIN                 6.2.0700 CORE EXT               53
    CASE                  6.2.0873 CORE EXT               54
    COMPILE,              6.2.0945 CORE EXT               54
    ENDCASE               6.2.1342 CORE EXT               54
    ENDOF                 6.2.1343 CORE EXT               55
    OF                    6.2.1950 CORE EXT               56
#endif
