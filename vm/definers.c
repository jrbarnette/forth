/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

#include "forth.h"

/*
 * definers.c - C utility functions for named definitions.
 */

/* Contains <name> execution semantics for the following:
 *------  ------  ------  ------  ------  ------  ------  ------
 * :                     6.1.0450 CORE
 * CONSTANT              6.1.0950 CORE
 * CREATE                6.1.1000 CORE
 * VARIABLE              6.1.2410 CORE
 *------  ------  ------  ------  ------  ------  ------  ------
 */


/* ( R: -- nest-sys ) initiation semantics */
DEFINER(do_colon, newip)
{
    CHECK_RPUSH(vm, 1);
    RPUSH(vm, (cell_ft)ip);
    return newip->vminstrs;
}


/* ( -- x ) name execution semantics */
DEFINER(do_constant, data_ptr)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, data_ptr->cell);
    return ip;
}


/* ( -- a-addr ) name execution semantics */
DEFINER(do_create, data_ptr)
{
    vmip_ft does_ptr = data_ptr[0].ip;
    addr_ft body = data_ptr[1].data;

    CHECK_PUSH(vm, 1);
    CHECK_RPUSH(vm, 1);
    PUSH(vm, (cell_ft) body);
    RPUSH(vm, ip);
    return does_ptr;
}


/* ( -- a-addr ) name execution semantics */
DEFINER(do_variable, var_addr)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, (cell_ft) var_addr);
    return ip;
}
