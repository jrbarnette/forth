/*
 * Copyright 2007, by J. Richard Barnette
 */

#ifndef VM_H
#define VM_H

#include <setjmp.h>
#include <stdlib.h>

#include "forth.h"

#define STACK_SIZE	2048
#define RSTACK_SIZE	64

typedef struct vminstr *	xt_ft;
typedef struct vmstate *	vmstate_p;

struct vmstate {
    jmp_buf	interp_loop;
    xt_ft *	ip;
    size_t	sp;
    size_t	rsp;
    cell_ft	stack[STACK_SIZE];
    cell_ft	rstack[RSTACK_SIZE];
};

typedef cell_ft (*vminstr_fn)(cell_ft, addr_ft, vmstate_p);

struct vminstr {
    vminstr_fn		handler;
    addr_unit_ft	data[1];
};

extern void overflow(vmstate_p);
extern void underflow(vmstate_p);

#define CHECK_UNDERFLOW(vm, n)	\
	    (((vm)->sp + (n) < STACK_SIZE) || (underflow(vm), 0))
#define CHECK_OVERFLOW(vm, n)	\
	    (((vm)->sp >= (n)) || (overflow(vm), 0))
#define CHECK_RPOP(vm, n)	\
	    (((vm)->rsp + (n) < RSTACK_SIZE) || (underflow(vm), 0))
#define CHECK_RPUSH(vm, n)	\
	    (((vm)->rsp >= (n)) || (overflow(vm), 0))

#define CLEAR_STACK(vm)		((vm)->sp = STACK_SIZE)
#define CLEAR_RSTACK(vm)	((vm)->rsp = RSTACK_SIZE)

#define POP(vm)		((vm)->stack[(vm)->sp++])
#define PUSH(vm, c)	((vm)->stack[--(vm)->sp] = (cell_ft)(c))
#define RPOP(vm)	((vm)->rstack[(vm)->rsp++])
#define RPUSH(vm, c)	((vm)->rstack[--(vm)->rsp] = (cell_ft)(c))

#endif
