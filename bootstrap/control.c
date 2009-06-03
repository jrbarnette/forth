/*
 * Copyright 2009, by J. Richard Barnette
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
static cell_ft
x_begin(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, tos);
    return (cell_ft) HERE;
}


/* ELSE			6.1.1310 CORE, p. 38 */
/* interpretation semantics undefined */
/* ( C: orig1 -- orig2 ) compilation semantics */
static cell_ft
x_else(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    vm_instr_p orig1 = (vm_instr_p) tos;
    vm_instr_p orig2;

    CHECK_POP(vm, 1);
    orig2 = compile_skip(vm, SKIP_XT);
    patch(orig1, (vm_instr_p) HERE);
    return (cell_ft) orig2;
}


/* IF			6.1.1700 CORE, p. 40 */
/* interpretation semantics undefined */
/* ( C: -- orig ) compilation semantics */
static cell_ft
x_if(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, tos);
    return (cell_ft) compile_skip(vm, FSKIP_XT);
}


/* REPEAT		6.1.2140 CORE, p. 45 */
/* interpretation semantics undefined */
/* ( C: orig dest -- ) compilation semantics */
static cell_ft
x_repeat(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    vm_instr_p dest = (vm_instr_p) tos;
    vm_instr_p orig;

    CHECK_POP(vm, 2);
    orig = (vm_instr_p) POP(vm);
    patch(compile_skip(vm, SKIP_XT), dest);
    patch(orig, (vm_instr_p) HERE);
    return (cell_ft) POP(vm);
}


/* THEN			6.1.2270 CORE, p. 47 */
/* interpretation semantics undefined */
/* ( C: orig -- ) compilation semantics */
static cell_ft
x_then(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_POP(vm, 1);
    patch((vm_instr_p) tos, (vm_instr_p) HERE);
    return (cell_ft) POP(vm);
}


/* UNTIL		6.1.2390 CORE, p. 48 */
/* interpretation semantics undefined */
/* ( C: dest -- ) compilation semantics */
static cell_ft
x_until(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    vm_instr_p dest = (vm_instr_p) tos;

    CHECK_POP(vm, 1);
    patch(compile_skip(vm, FSKIP_XT), dest);
    return POP(vm);
}


/* WHILE		6.1.2430 CORE, p. 49 */
/* interpretation semantics undefined */
/* ( C: dest -- orig dest ) compilation semantics */
static cell_ft
x_while(cell_ft tos, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    CHECK_POP(vm, 1);
    PUSH(vm, compile_skip(vm, FSKIP_XT));
    return tos;
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
