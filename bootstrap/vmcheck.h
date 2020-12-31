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
inline vmip_ft
throw_transfer(vmstate_ft *vm, cell_ft throw_code)
{
    if (!HAVE_CATCH(vm)) {
	THROW(vm, throw_code);
	return NULL;
    }

    vm->rsp = vm->catch_rsp;
    vm->catch_rsp = (sp_ft) RPOP(vm);
    vm->sp = (sp_ft) RPOP(vm);
    PICK(SP(vm), 0) = throw_code;

    return (vmip_ft) RPOP(vm);
}


#define CHECK(vm, t, x)	    { if (t) return throw_transfer((vm), (x)); }
#define NOCHECK(vm, t, x)

#ifndef STACKCHECK
#define STACKCHECK	CHECK
#endif

#define CHECK_PUSH(vm, n)	STACKCHECK(vm, (n) > CANPUSH(vm), -3)
#define CHECK_POP(vm, n)	STACKCHECK(vm, (n) > CANPOP(vm), -4)
#define CHECK_RPUSH(vm, n)	STACKCHECK(vm, (n) > RCANPUSH(vm), -5)
#define CHECK_RPOP(vm, n)	STACKCHECK(vm, (n) > RCANPOP(vm), -6)

#endif // VMCHECK_H
