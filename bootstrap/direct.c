/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

#include <stdlib.h>

#include "forth.h"

/*
 * direct.c - C utility functions to enable direct-threaded
 *   interpretation.
 */


void
direct_execute(vmstate_ft *vm, vmip_ft ip)
{
    RPUSH(vm, NULL);

    int throwcode;
    while ((throwcode = CATCH(vm)) != 0) {
	// this logic is partly replicated in throw_transfer()
	if (EXEMPTY(vm)) {
	    report_exception(throwcode, vm, NULL);
	    abort();
	}
	ip = throw_transfer(vm, throwcode);
    }

    while (ip != NULL) {
	ip = ip->handler(ip + 1, vm, NULL);
    }
}


/* ( R: -- ip ) */
PRIM_HDLR(i_call)
{
    CHECK_RPUSH(vm, 1);
    RPUSH(vm, ip + 1);
    return ip->ip;
}


/* ( x*i xt -- x*j ) */
PRIM_HDLR(i_indirect)
{
    CHECK_POP(vm, 1);
    execute(vm, (xt_ft) POP(vm));
    return ip;
}
