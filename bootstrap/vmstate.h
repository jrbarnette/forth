/*
 * Copyright 2020, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef VMSTATE_H
#define VMSTATE_H

#include <setjmp.h>
#include <stddef.h>

/*
 * Virtual Machine State
 *
 * The virtual machine interpreter uses a more-or-less traditional
 * indirect threaded approach:  A Forth definition consists of a
 * pointer to a handler function that executes the defintion,
 * followed by definition data.  The definition is invoked by
 * calling the handler with the VM state and the address of the
 * definition data.
 *
 * The "execution token" for a definition is simply its address.
 * The execution token also serves as the representation of a
 * virtual machine instruction.
 *
 * The defintion data for a colon definition is an array of
 * virtual machine instructions (execution tokens) to be executed in
 * order.  The execution logic and the data structures come together
 * in these key functions: execute(), do_colon(), x_exit().
 */

#define STACK_SIZE	256
#define RSTACK_SIZE	512

typedef struct vmstate {
    a_addr_ft	sp;
    a_addr_ft	rsp;
    a_addr_ft	catch_handler;
    cell_ft	stack[STACK_SIZE];
    cell_ft	rstack[RSTACK_SIZE];
    jmp_buf	interp_loop;
} *vmstate_p;


#define CLEAR_STACK(vm)		((vm)->sp = (vm)->stack + STACK_SIZE)
#define CLEAR_RSTACK(vm)	((vm)->rsp = (vm)->rstack + RSTACK_SIZE)
#define SFREE(vm)		((vm)->sp - (vm)->stack)
#define RSFREE(vm)		((vm)->rsp - (vm)->rstack)
#define DEPTH(vm)		(STACK_SIZE - SFREE(vm))
#define RDEPTH(vm)		(RSTACK_SIZE - RSFREE(vm))
#define EMPTY(vm)		(SFREE(vm) == STACK_SIZE)
#define REMPTY(vm)		(RSFREE(vm) == RSTACK_SIZE)
#define SP(vm)			((vm)->sp)
#define RSP(vm)			((vm)->rsp)
#define PICK(sp, n)		((sp)[(n)])
#define SET_SP(vm, nsp, n)	((vm)->sp = (nsp) + (n))
#define SET_RSP(vm, nsp, n)	((vm)->rsp = (nsp) + (n))

#define POP(vm)			(*(vm)->sp++)
#define PUSH(vm, c)		(*--(vm)->sp = (cell_ft) (c))
#define RPOP(vm)		(*(vm)->rsp++)
#define RPUSH(vm, c)		(*--(vm)->rsp = (cell_ft) (c))

#define CATCH(vm)		(setjmp((vm)->interp_loop))
#define THROW(vm, n)		(longjmp((vm)->interp_loop, (int) (n)))


inline void
vm_initialize(vmstate_p vm) {
    CLEAR_STACK(vm);
    CLEAR_RSTACK(vm);
    vm->catch_handler = NULL;
}

#endif // VMSTATE_H
