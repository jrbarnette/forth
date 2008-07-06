/*
 * Copyright 2008, by J. Richard Barnette
 */

#include "forth.h"

static void
add_defns(defn_dt *dp)
{
    name_p nm;

    while (dp->name != NULL) {
	nm = addname(dp->name, dp->fn);
	if (dp->definer != NULL) {
	    dp->definer(nm, dp->datap);
	}

	dp++;
    }
}

void
init_forth(void)
{
    add_defns(dictionary_defns);
    add_defns(primitive_defns);
    add_defns(compile_defns);
}
