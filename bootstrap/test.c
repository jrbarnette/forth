/*
 * Copyright 2008, by J. Richard Barnette
 */

#include <stdlib.h>

#include "forth.h"

static struct vmstate vmstate;

int
main(int argc, char *argv[])
{
    init_forth();

    return EXIT_SUCCESS;
}
