\ Copyright 2015, by J. Richard Barnette, All Rights Reserved.

C-CODE>
#include <stddef.h>

#include "forth.h"
#include "direct.h"

union {
    cell_ft		cell;
    char		cdata[sizeof (cell_ft)];
    addr_ft		ref;
    vminstr_fn		handler;
} raw_dictionary[] = {
END-CODE
    flush-target-dictionary
C-CODE>
};

DIRECT_FORTH(initialize_forth) // {
        L(raw_dictionary) L(dictionary.dict_space)
        L(sizeof (raw_dictionary))
        X(x_move)
END_DIRECT // }
END-CODE
