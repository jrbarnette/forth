#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>

#include "bench.h"

#define STACK_SIZE	2048
#define RSTACK_SIZE	64

typedef struct vminstr *	xt_ft;
typedef struct vmstate *	vmstate_p;

struct vmstate {
    xt_ft	ip;
    cell_ft *	sp;
    cell_ft *	rsp;
    cell_ft	stack[STACK_SIZE];
    cell_ft	rstack[RSTACK_SIZE];
};

typedef xt_ft (*vminstr_fn)(xt_ft, vmstate_p);

struct vminstr {
    vminstr_fn	handler;
};

void
execute_direct(vmstate_p vm, xt_ft entry_xt)
{
    xt_ft ip;

    ip = entry_xt->handler(NULL, vm);

    while (ip != NULL) {
	xt_ft xtok = ip++;
	ip = xtok->handler(ip, vm);
    }
}
