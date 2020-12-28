/*
 * Copyright 2020, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef VMCHECK_H
#define VMCHECK_H

/*
 * Definitions to allow VM XT handler functions to check for and throw
 * errors.
 */

/*
 * Set VM state for a transfer of control due to THROW.  Note that
 * this function's behavior is coupled to the primitives do_catch(),
 * undo_catch(), and x_throw() in vm.c.
 */
inline vminstr_p
throw_transfer(vmstate_p vm, cell_ft throw_code)
{
    if (vm->catch_handler == NULL) {
        THROW(vm, throw_code);
	return NULL;
    }

    vm->rsp = vm->catch_handler;
    cell_ft *rsp = RSP(vm);

    vminstr_p newip = (vminstr_p) PICK(rsp, 2);
    vm->sp = (cell_ft *) PICK(rsp, 1);
    vm->catch_handler = (cell_ft *) PICK(rsp, 0);
    SET_RSP(vm, rsp, 3);

    PICK(SP(vm), 0) = throw_code;

    return newip;
}


#define CHECK(vm, t, x)	    { if (t) return throw_transfer((vm), (x)); }
#define NOCHECK(vm, t, x)

#ifndef STACKCHECK
#define STACKCHECK	CHECK
#endif

#define CHECK_PUSH(vm, n)	STACKCHECK(vm, (n) > SFREE(vm), -3)
#define CHECK_POP(vm, n)	STACKCHECK(vm, (n) > DEPTH(vm), -4)
#define CHECK_RPUSH(vm, n)	STACKCHECK(vm, (n) > RSFREE(vm), -5)
#define CHECK_RPOP(vm, n)	STACKCHECK(vm, (n) > RDEPTH(vm), -6)

#endif // VMCHECK_H
