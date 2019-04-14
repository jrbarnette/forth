/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

#include "forth.h"

/*
 * direct.c - C utility functions to enable direct-threaded
 *   interpretation.
 */


static void
direct(vmstate_p vm, vminstr_p ip)
{
    RPUSH(vm, NULL);
    while (ip != NULL) {
	ip = ip->handler(ip + 1, vm, NULL);
    }
}
