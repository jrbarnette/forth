\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\  dictionary.fth - Primitives for standard Forth words for
\      dictionary allocation.

expr sizeof (DICT); expr &DICT.here;    !
1 literal expr &DICT.n_search_order;    !
expr &DICT.forth_wordlist;
    dup expr &DICT.current;             !
    expr &DICT.search_order[0];         !

prim: ,         x_comma
prim: ALIGN     x_align
prim: ALLOT     x_allot
prim: C,        x_c_comma
prim: HERE      x_here
prim: UNUSED    x_unused
