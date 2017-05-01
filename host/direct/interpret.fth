\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\  interpret.c - Outer (interactive) interpreter, and related Forth
\    words.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  >IN                   6.1.0560 CORE                   31
\  ABORT                 6.1.0670 CORE                   32
\  BASE                  6.1.0750 CORE                   34
\  CHAR                  6.1.0895 CORE                   35
\  DECIMAL               6.1.1170 CORE                   36
\  EVALUATE              6.1.1360 CORE                   38
\  EXECUTE               6.1.1370 CORE                   38
\  LITERAL               6.1.1780 CORE                   41
\  POSTPONE              6.1.2033 CORE                   43
\  QUIT                  6.1.2050 CORE                   43
\  S"                    6.1.2165 CORE                   44
\  SOURCE                6.1.2216 CORE                   45
\  STATE                 6.1.2250 CORE                   45
\  [                     6.1.2500 CORE                   48
\  ]                     6.1.2540 CORE                   49
\
\  C"                    6.2.0855 CORE EXT               52
\  HEX                   6.2.1660 CORE EXT               54
\  PARSE                 6.2.2008 CORE EXT               55
\  REFILL                6.2.2125 CORE EXT               55
\ ------  ------  ------  ------  ------  ------  ------  ------

expr do_literal;  expr DO_LITERAL_XT;   !
expr do_postpone; expr DO_POSTPONE_XT;  !
expr do_s_quote;  expr S_QUOTE_XT;      !
expr do_c_quote;  expr C_QUOTE_XT;      !

10 literal        expr &DICT.base;      !

prim: >IN       x_to_in
prim: ABORT     x_abort
prim: BASE      x_base
prim: CHAR      x_char
prim: DECIMAL   x_decimal
prim: EVALUATE  x_evaluate
prim: EXECUTE   x_execute
prim: LITERAL   x_literal               compile-only
prim: POSTPONE  x_postpone              compile-only
prim: QUIT      x_quit
prim: SOURCE    x_source
prim: STATE     x_state
prim: S"        x_s_quote               compile-only
prim: [         x_left_bracket          compile-only
prim: ]         x_right_bracket

prim: C"        x_c_quote               compile-only
prim: HEX       x_hex
prim: PARSE     x_parse
prim: REFILL    x_refill
