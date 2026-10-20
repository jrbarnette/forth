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
static inline vmip_ft
throw_transfer(vmstate_ft *vm, cell_ft throw_code)
{
    if (EXEMPTY(vm)) {
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
#define NOCHECK(vm, t, x)   ((void) (t))

#ifndef STACKCHECK
#define STACKCHECK	CHECK
#endif

#ifdef STACKPROFILE
#define SPROBE(vm, n)	(probe_push((vm), (n)))
#define RPROBE(vm, n)	(probe_rpush((vm), (n)))
#else
#define SPROBE(vm, n)	((n) > MAXPUSH(vm))
#define RPROBE(vm, n)	((n) > MAXRPUSH(vm))
#endif

#define CHECK_PUSH(vm, n)	STACKCHECK(vm, SPROBE(vm, n), -3)
#define CHECK_POP(vm, n)	STACKCHECK(vm, (n) > MAXPOP(vm), -4)
#define CHECK_RPUSH(vm, n)	STACKCHECK(vm, RPROBE(vm, n), -5)
#define CHECK_RPOP(vm, n)	STACKCHECK(vm, (n) > MAXRPOP(vm), -25)

#endif // VMCHECK_H
