/*
 * Copyright 2007, by J. Richard Barnette
 */

#include "forth.h"

/* -------------------------------------------------------------- */
static cell_t
x_literal(xt ignore, cell_t tos, vmstate_p vm)
{
    /* ( -- x ) */
    CHECK_SPUSH(vm, 1);
    PUSH(vm, tos);
    return (cell_t) *vm->ip++;
}
static definition
xx_literal = { "x-literal", x_literal, 0 };

