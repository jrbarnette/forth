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
typedef struct definition_data *	xt_ft;
typedef union instruction_data		vminstr_d;
typedef union instruction_data *	vminstr_p;
typedef union parameter_data		vmarg_d;
typedef union parameter_data *		vmarg_p;

typedef vminstr_p (*vminstr_fn)(vminstr_p, vmstate_p, vmarg_p);

/* Now resolve the forward references in dependency order */
union instruction_data {
    /* general purpose VM instructions */
    xt_ft		xtok;
    cell_ft		cell;
    snumber_ft		offset;
    char_ft		cdata[CELL_SIZE / CHAR_SIZE];
    addr_unit_ft	data[CELL_SIZE];

    /* direct execution only */
    vminstr_fn		handler;
    vminstr_p		ip;
    char *		id;
};

union parameter_data {
    vminstr_p		ip;
    xt_ft		xtok;
    cell_ft		cell;
    char_ft		cdata[CELL_SIZE / CHAR_SIZE];
    addr_unit_ft	data[CELL_SIZE];
    vminstr_d		vminstrs[1];
};

struct definition_data {
    vminstr_fn		handler;
    vmarg_d		arg[1];
};


#endif // XTOKEN_H
