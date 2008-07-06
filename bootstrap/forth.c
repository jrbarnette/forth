/*
 * Copyright 2007, by J. Richard Barnette
 */

#include <stdlib.h>

#include "forth.h"

static struct vmstate vmstate;

int
main(int argc, char *argv[])
{
    name_p	quit;
    init_forth();

    quit = lookup("QUIT");
    interpret(&vmstate, NAME_CFA(quit));

    return EXIT_SUCCESS;
}
