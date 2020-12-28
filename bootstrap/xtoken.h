/*
 * Copyright 2020, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef XTOKEN_H
#define XTOKEN_H

/*
 * Definition of execution tokens for indirect interpretation.  An
 * execution token is just a pointer to a simple structure
 * containing a pointer to a handler function followed by one or
 * more parameter cells.
 *
 * An XT handler function takes three parameters:
 *   + A pointer to the next VM IP.
 *   + A pointer to the VM state to be operated on.
 *   + A pointer to the XT parameter cells.
 * The handlers return a pointer to the VM IP where execution should
 * continue.  Handlers that don't perform any kind of flow of
 * control change should just return the passed in IP.
 * Interpretation stops when a handler returns NULL.
 *
 * The actual C declarations are complicated because the underlying
 * types involve quite a few forward references.  The basic types
 * are these:
 * 
 * xt:		( xt ) An execution token as defined in the standard.
 * arg:		One cell in an execution token's parameter field.
 *              These are accessible to defintions defined by CREATE.
 * instr:	Content addressed by a VM IP.  This concept doesn't
 *              directly exist in the standard.
 * handler:	XT handler function.  This concept doesn't directly
 *              exist in the standard.
 */

/* Our basic types need forward references to incomplete structures */
typedef struct definition *		xt_ft;
typedef union instruction_cell		vminstr_ft;
typedef union instruction_cell *	vmip_ft;
typedef union parameter_cell		vmarg_ft;

#define DEFINER(hdlr, arg) 	\
    vmip_ft hdlr(vmip_ft ip, vmstate_ft *vm, vmarg_ft *arg)

#define PRIM_HDLR(hdlr)		DEFINER(hdlr, ignore)

typedef PRIM_HDLR((*vmhdlr_fn));

/* Now resolve the forward references in dependency order */
union instruction_cell {
    /* general purpose VM instructions */
    xt_ft		xtok;
    cell_ft		cell;
    snumber_ft		offset;
    char_ft		cdata[CELL_SIZE / CHAR_SIZE];
    addr_unit_ft	data[CELL_SIZE];

    /* direct execution only */
    vmhdlr_fn		handler;
    vmip_ft		ip;
    char *		id;
};

union parameter_cell {
    vmip_ft		ip;
    cell_ft		cell;
    addr_unit_ft	data[CELL_SIZE];
    vminstr_ft		vminstrs[1];
};

struct definition {
    vmhdlr_fn		handler;
    vmarg_ft		arg[1];
};

#endif // XTOKEN_H
