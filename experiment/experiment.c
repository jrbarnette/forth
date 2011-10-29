#include <stddef.h>
#include <setjmp.h>

#define STACK_SIZE	2048
#define RSTACK_SIZE	64

typedef union vminstr *		xt_ft;
typedef struct vmstate *	vmstate_p;

struct vmstate {
    jmp_buf	interp_loop;
    xt_ft *	ip;
    unsigned *	sp;
    unsigned *	rsp;
    unsigned	stack[STACK_SIZE];
    unsigned	rstack[RSTACK_SIZE];
};

typedef unsigned (*vminstr_fn_v1)(unsigned, vmstate_p, void *);
typedef xt_ft *(*vminstr_fn_v2)(xt_ft *, vmstate_p, void *);

union vminstr {
    vminstr_fn_v1	handler_v1;
    vminstr_fn_v2	handler_v2;
    void *		data[1];
    int			snum;
    unsigned		cell;
};

/* ---- */

void
execute_v1(vmstate_p vm, xt_ft entry_xt)
{
    unsigned tos;

    tos = entry_xt->handler_v1(vm->stack[STACK_SIZE-1], vm, entry_xt[1].data);

    while (vm->ip != NULL) {
	xt_ft xtok = *vm->ip++;
	tos = xtok->handler_v1(tos, vm, xtok[1].data);
    }

    vm->stack[STACK_SIZE-1] = tos;
}

unsigned
unary_v1(unsigned tos, vmstate_p vm, void *ignore)
{
    return ~tos;
}

unsigned
binary_v1(unsigned tos, vmstate_p vm, void *ignore)
{
    return tos + *vm->sp++;
}

unsigned
dup_v1(unsigned tos, vmstate_p vm, void *ignore)
{
    *--vm->sp = tos;
    return tos;
}

unsigned
swap_v1(unsigned tos, vmstate_p vm, void *ignore)
{
    unsigned ntos = *vm->sp;
    *vm->sp = tos;
    return ntos;
}

unsigned
drop_v1(unsigned tos, vmstate_p vm, void *ignore)
{
    return *vm->sp++;
}

unsigned
variable_v1(unsigned tos, vmstate_p vm, void *addr)
{
    *--vm->sp = tos;
    return (unsigned) addr;
}

unsigned
fetch_v1(unsigned tos, vmstate_p vm, void *ignore)
{
    return *(unsigned *)tos;
}

unsigned
store_v1(unsigned tos, vmstate_p vm, void *ignore)
{
    unsigned *sp = vm->sp;
    unsigned data = sp[0];
    *(unsigned *)tos = data;
    vm->sp = sp + 2;
    return sp[1];
}

unsigned
do_colon_v1(unsigned tos, vmstate_p vm, void *newip)
{
    *--vm->rsp = (unsigned) vm->ip;
    vm->ip = (xt_ft *) newip;
    return tos;
}

unsigned
do_exit_v1(unsigned tos, vmstate_p vm, void *ignore)
{
    vm->ip = (xt_ft *) *vm->rsp++;
    return tos;
}

/* ---- */

void
execute_v2(vmstate_p vm, xt_ft entry_xt)
{
    xt_ft *ip;

    ip = entry_xt->handler_v2(vm->ip, vm, entry_xt[1].data);

    while (ip != NULL) {
	xt_ft xtok = *ip++;
	ip = xtok->handler_v2(ip, vm, xtok[1].data);
    }

    vm->ip = ip;
}

xt_ft *
unary_v2(xt_ft *ip, vmstate_p vm, void *ignore)
{
    unsigned *sp = vm->sp;
    *sp = ~*sp;
    return ip;
}

xt_ft *
binary_v2(xt_ft *ip, vmstate_p vm, void *ignore)
{
    unsigned *sp = vm->sp + 1;
    sp[0] += sp[-1];
    vm->sp = sp;
    return ip;
}

xt_ft *
dup_v2(xt_ft *ip, vmstate_p vm, void *ignore)
{
    unsigned t = *vm->sp;
    *--vm->sp = t;
    return ip;
}

xt_ft *
swap_v2(xt_ft *ip, vmstate_p vm, void *ignore)
{
    unsigned *sp = vm->sp;
    unsigned sp0 = sp[0];
    unsigned sp1 = sp[1];
    sp[1] = sp0; sp[0] = sp1;
    return ip;
}

xt_ft *
drop_v2(xt_ft *ip, vmstate_p vm, void *ignore)
{
    vm->sp++;
    return ip;
}

xt_ft *
variable_v2(xt_ft *ip, vmstate_p vm, void *addr)
{
    *--vm->sp = (unsigned) addr;
    return ip;
}

xt_ft *
fetch_v2(xt_ft *ip, vmstate_p vm, void *ignore)
{
    unsigned *sp = vm->sp;
    *sp = *(unsigned *) *sp;
    return ip;
}

xt_ft *
store_v2(xt_ft *ip, vmstate_p vm, void *ignore)
{
    unsigned *sp0 = vm->sp;
    unsigned *sp1 = sp0 + 1;
    unsigned *addr = (unsigned *) *sp0;
    unsigned data = *sp1;
    vm->sp = sp1; *addr = data;
    return ip;
}

xt_ft *
do_colon_v2(xt_ft *ip, vmstate_p vm, void *newip)
{
    *--vm->rsp = (unsigned) ip;
    return (xt_ft *) newip;
}

xt_ft *
do_exit_v2(xt_ft *ip, vmstate_p vm, void *ignore)
{
    return (xt_ft *) *vm->rsp++;
}
