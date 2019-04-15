/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>
#include <stdlib.h>

#include "forth.h"

/*
 * direct.c - C utility functions to enable direct-threaded
 *   interpretation.
 */


void
direct_execute(vmstate_p vm, vminstr_p ip)
{
    CLEAR_STACK(vm);
    CLEAR_RSTACK(vm);

    volatile int	throwcode;
    if ((throwcode = CATCH(vm)) != 0) {
	handle_exception(throwcode, vm, NULL);
	abort();
    }

    RPUSH(vm, NULL);
    while (ip != NULL) {
	ip = ip->handler(ip + 1, vm, NULL);
    }
}
