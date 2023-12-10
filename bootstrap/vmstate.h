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
 * The VM state structure contains these elements:
 *   sp - pointer to the top of the data stack.
 *   rsp - pointer to the top of the return stack.
 *   catch_rsp - pointer to the top exception stack frame.
 *   stack - space for the data stack.
 *   rstack - space for the return stack.
 *   interp_loop - a jmp_buf that is set to return to outside of the
 *     main VM interpreter loop
 *
 * Stacks grow down in memory.
 *
 * The exception stack is stored on the return stack.  Return stack
 * bounds checks consider the top frame of the exception stack to be
 * the bottom of the return stack (see the MAXRPOP macro, below).
 * The exception stack is considered empty when it points to the
 * base of the return stack.
 *
 * The macros in this header are meant to encapsulate certain
 * implementation details from the rest of the system, specifically:
 *   + Whether the stack grows up or down in memory.
 *   + Whether the stack pointers are actual pointers, or
 *     indexes into an array of cells.
 *   + Whether stack pointers address the cell at top of stack or
 *     the first empty cell.
 *
 * There are macros for addressing the stack directly as an array of
 * cells:
 *   SP(vm) - The pointer to the top of the data stack.
 *   RSP(vm) - The pointer to the top of the return stack.
 *   PICK(sp, n) - An lvalue addressing the nth cell on a stack as
 *     returned by SP() or RSP().  PICK(sp, 0) is top-of-stack.
 *     Positive values of n address values below top of stack;
 *     negative values address empty slots above TOS.
 *   SET_SP(vm, nsp, n) - Set the data stack pointer to point to
 *     the cell addressed by PICK(nsp, n).
 *   SET_RSP(vm, nsp, n) - Set the return stack pointer to point to
 *     the cell addressed by PICK(nsp, n).
 */

#define STACK_SIZE	256
#define RSTACK_SIZE	512

typedef cell_ft *sp_ft;

typedef struct {
    sp_ft	sp;
    sp_ft	rsp;
    sp_ft	catch_rsp;
    cell_ft	stack[STACK_SIZE];
    cell_ft	rstack[RSTACK_SIZE];
    jmp_buf	interp_loop;
} vmstate_ft;


#define STACK_BASE(vm)		((vm)->stack + STACK_SIZE)
#define RSTACK_BASE(vm)		((vm)->rstack + RSTACK_SIZE)

#define CLEAR_STACK(vm)		((vm)->sp = STACK_BASE(vm))
#define CLEAR_RSTACK(vm)	((vm)->catch_rsp = (vm)->rsp = RSTACK_BASE(vm))

#define EMPTY(vm)		((vm)->sp == STACK_BASE(vm))
#define REMPTY(vm)		((vm)->rsp == RSTACK_BASE(vm))
#define EXEMPTY(vm)		((vm)->catch_rsp == RSTACK_BASE(vm))

#define MAXPUSH(vm)		((vm)->sp - (vm)->stack)
#define MAXPOP(vm)		(STACK_SIZE - MAXPUSH(vm))
#define MAXRPUSH(vm)		((vm)->rsp - (vm)->rstack)
#define MAXRPOP(vm)		((vm)->catch_rsp - (vm)->rsp)

#define POP(vm)			(*(vm)->sp++)
#define PUSH(vm, c)		(*--(vm)->sp = (cell_ft) (c))
#define RPOP(vm)		(*(vm)->rsp++)
#define RPUSH(vm, c)		(*--(vm)->rsp = (cell_ft) (c))

#define SP(vm)			((vm)->sp)
#define RSP(vm)			((vm)->rsp)
#define PICK(sp, n)		((sp)[(n)])
#define SET_SP(vm, nsp, n)	((vm)->sp = (nsp) + (n))
#define SET_RSP(vm, nsp, n)	((vm)->rsp = (nsp) + (n))

#define CATCH(vm)		(setjmp((vm)->interp_loop))
#define THROW(vm, n)		(longjmp((vm)->interp_loop, (int) (n)))


static inline void
vm_initialize(vmstate_ft *vm) {
    CLEAR_STACK(vm);
    CLEAR_RSTACK(vm);
}

#endif // VMSTATE_H
