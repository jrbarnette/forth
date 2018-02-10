\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\  control.fth - Standard Forth words relating to flow of control in
\    compiled defintions.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  +LOOP                 6.1.0140 CORE                   27
\  BEGIN                 6.1.0760 CORE                   34
\  DO                    6.1.1240 CORE                   36
\  ELSE                  6.1.1310 CORE                   37
\  I                     6.1.1680 CORE                   39
\  IF                    6.1.1700 CORE                   40
\  J                     6.1.1730 CORE                   40
\  LEAVE                 6.1.1760 CORE                   41
\  REPEAT                6.1.2140 CORE                   44
\  THEN                  6.1.2270 CORE                   46
\  UNLOOP                6.1.2380 CORE                   47
\  UNTIL                 6.1.2390 CORE                   47
\  WHILE                 6.1.2430 CORE                   47
\ ------  ------  ------  ------  ------  ------  ------  ------

<C> do_do;         <C> DO_DO_XT;      !
<C> do_plus_loop;  <C> PLUS_LOOP_XT;  !
<C> do_skip;       <C> SKIP_XT;       !
<C> do_fskip;      <C> FSKIP_XT;      !

prim: UNLOOP    x_unloop        no-interpret

prim: +LOOP     x_plus_loop     compile-only
[compile] UNLOOP

prim: BEGIN     x_begin         compile-only
prim: DO        x_do            compile-only
prim: ELSE      x_else          compile-only
prim: I         x_i             no-interpret
prim: IF        x_if            compile-only
prim: J         x_j             no-interpret
prim: LEAVE     c_leave         compile-only

prim: REPEAT    x_repeat        compile-only
prim: THEN      x_then          compile-only
prim: UNTIL     x_until         compile-only
prim: WHILE     x_while         compile-only
