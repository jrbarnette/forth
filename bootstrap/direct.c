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
    vm_initialize(vm);
    RPUSH(vm, NULL);

    int throwcode;
    while ((throwcode = CATCH(vm)) != 0) {
	if (vm->catch_rsp == NULL) {
	    report_exception(throwcode, vm, NULL);
	    abort();
	}
	ip = throw_transfer(vm, throwcode);
    }

    while (ip != NULL) {
	ip = ip->handler(ip + 1, vm, NULL);
    }
}
