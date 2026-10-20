/*
 * Copyright 2025, by J. Richard Barnette. All Rights Reserved.
 */

#include "forth.h"
#include "terminal.h"


/*
 * termops.c - Standard Forth words for user terminal I/O.
 */

/*------  ------  ------  ------  ------  ------  ------  ------
 * EMIT                  6.1.1320 CORE                   38
 * ACCEPT                6.1.0695 CORE                   33
 *------  ------  ------  ------  ------  ------  ------  ------
 */


/* ( x -- ) */
PRIM_HDLR(x_emit)
{
    CHECK_POP(vm, 1);
    term_emit((char) POP(vm));
    return ip;
}


/* ( c-addr u prompt -- len flag ) */
PRIM_HDLR(x_prompt_accept)
{
    CHECK_POP(vm, 3);

    cell_ft *sp = SP(vm);
    char *buff = (char *) PICK(sp, 2);
    cell_ft buff_len = PICK(sp, 1);
    char *prompt = (char *) PICK(sp, 0);

    PICK(sp, 1) = term_readline(prompt, buff, &buff_len);
    PICK(sp, 2) = buff_len;

    SET_SP(vm, sp, 1);
    return ip;
}
