/*
 * Copyright 2017, by J. Richard Barnette. All Rights Reserved.
 */

#include "forth.h"
#include "direct.h"


static
DIRECT_FORTH(init_dictionary) // {
    L(sizeof (dictionary.dict_static_data))
	L(&DICT.here)			X(x_store)
    L(1)
	L(&DICT.n_search_order)		X(x_store)
    L(&DICT.forth_wordlist) X(x_dup)
	L(&DICT.current)		X(x_store)
	L(&DICT.search_order[0])	X(x_store)

    PRIM(",",         x_comma)
    PRIM("ALIGN",     x_align)
    PRIM("ALLOT",     x_allot)
    PRIM("C,",        x_c_comma)
    PRIM("HERE",      x_here)
    PRIM("UNUSED",    x_unused)
END_DIRECT // }


static
DIRECT_FORTH(init_stack_prim) // {
    PRIM(">R",		x_to_r)			FLAGS(NO_INTERPRET)
    PRIM("?DUP",	x_question_dup)
    PRIM("DEPTH",	x_depth)
    PRIM("DROP",	x_drop)
    PRIM("DUP",		x_dup)
    PRIM("OVER",	x_over)
    PRIM("R>",		x_r_from)		FLAGS(NO_INTERPRET)
    PRIM("R@",		x_r_fetch)		FLAGS(NO_INTERPRET)
    PRIM("ROT",		x_rot)
    PRIM("SWAP",	x_swap)

    PRIM("2>R",		x_two_to_r)		FLAGS(NO_INTERPRET)
    PRIM("2R>",		x_two_r_from)		FLAGS(NO_INTERPRET)
    PRIM("2R@",		x_two_r_fetch)		FLAGS(NO_INTERPRET)
    PRIM("PICK",	x_pick)
END_DIRECT // }


static
DIRECT_FORTH(init_arith_prim) // {
    PRIM("+",		x_plus)
    PRIM("-",		x_minus)
    PRIM("2*",		x_two_star)
    PRIM("2/",		x_two_slash)
    PRIM("<",		x_less_than)
    PRIM("=",		x_equals)
    PRIM(">",		x_greater_than)
    PRIM("AND",		x_and)
    PRIM("INVERT",	x_invert)
    PRIM("LSHIFT",	x_lshift)
    PRIM("NEGATE",	x_negate)
    PRIM("OR",		x_or)
    PRIM("RSHIFT",	x_rshift)
    PRIM("U<",		x_u_less)
    PRIM("XOR",		x_xor)
END_DIRECT // }


static
DIRECT_FORTH(init_memory_prim) // {
    PRIM("!", 		x_store)
    PRIM("@",		x_fetch)
    PRIM("C!",		x_c_store)
    PRIM("C@",		x_c_fetch)
    PRIM("MOVE",	x_move)
END_DIRECT // }


static
DIRECT_FORTH(init_mult_prim) // {
    PRIM("*",		x_star)
    PRIM("/",		x_slash)
    PRIM("/MOD",	x_slash_mod)
    PRIM("M*",		x_m_star)
    PRIM("MOD",		x_mod)
    PRIM("UM*",		x_u_m_star)
    PRIM("UM/MOD",	x_u_m_slash_mod)
END_DIRECT // }


static
DIRECT_FORTH(init_terminal_prim) // {
    PRIM("EMIT", x_emit)
END_DIRECT // }


static
DIRECT_FORTH(init_names) // {
    L(do_does)  L(DOES_XT)  X(x_store)

    PRIM("'",			x_tick)
    PRIM(":",			x_colon)

    /* EXIT out of order for reference below */
    PRIM("EXIT",		x_exit)		FLAGS(NO_INTERPRET)

    PRIM(";",			x_semicolon)	FLAGS(COMPILE)
    XCOMPILE("EXIT")

    PRIM(">BODY",		x_to_body)
    PRIM("CONSTANT",		x_constant)
    PRIM("CREATE",		x_create)
    PRIM("DOES>",		x_does)		FLAGS(COMPILE)
    PRIM("VARIABLE",		x_variable)

    PRIM("FORTH-WORDLIST",	x_forth_wordlist)
    PRIM("SEARCH-WORDLIST",	x_search_wordlist)
    PRIM("FIND",		x_find)
END_DIRECT // }


static
DIRECT_FORTH(init_control) // {
    L(do_do)         L(DO_DO_XT)      X(x_store)
    L(do_plus_loop)  L(PLUS_LOOP_XT)  X(x_store)
    L(do_skip)       L(SKIP_XT)       X(x_store)
    L(do_fskip)      L(FSKIP_XT)      X(x_store)

    PRIM("UNLOOP",	x_unloop)	FLAGS(NO_INTERPRET)

    PRIM("+LOOP",	x_plus_loop)	FLAGS(COMPILE)
    XCOMPILE("UNLOOP")

    PRIM("BEGIN",	x_begin)	FLAGS(COMPILE)
    PRIM("DO",		x_do)		FLAGS(COMPILE)
    PRIM("ELSE",	x_else)		FLAGS(COMPILE)
    PRIM("I",		x_i)		FLAGS(NO_INTERPRET)
    PRIM("IF",		x_if)		FLAGS(COMPILE)
    PRIM("J",		x_j)		FLAGS(NO_INTERPRET)
    PRIM("LEAVE",	c_leave)	FLAGS(COMPILE)

    PRIM("REPEAT",	x_repeat)	FLAGS(COMPILE)
    PRIM("THEN",	x_then)		FLAGS(COMPILE)
    PRIM("UNTIL",	x_until)	FLAGS(COMPILE)
    PRIM("WHILE",	x_while)	FLAGS(COMPILE)
END_DIRECT // }


static
DIRECT_FORTH(init_interpret) // {
    L(do_literal)  L(DO_LITERAL_XT)  X(x_store)
    L(do_postpone) L(DO_POSTPONE_XT) X(x_store)
    L(do_s_quote)  L(S_QUOTE_XT)     X(x_store)
    L(do_c_quote)  L(C_QUOTE_XT)     X(x_store)

    L(10)          L(&DICT.base)     X(x_store)

    PRIM(">IN",		x_to_in)
    PRIM("ABORT",	x_abort)
    PRIM("BASE",	x_base)
    PRIM("CHAR",	x_char)
    PRIM("DECIMAL",	x_decimal)
    PRIM("EVALUATE",	x_evaluate)
    PRIM("EXECUTE",	x_execute)
    PRIM("LITERAL",	x_literal)		FLAGS(COMPILE)
    PRIM("POSTPONE",	x_postpone)		FLAGS(COMPILE)
    PRIM("QUIT",	x_quit)
    PRIM("SOURCE",	x_source)
    PRIM("STATE",	x_state)
    PRIM("S\"",		x_s_quote)		FLAGS(COMPILE)
    PRIM("[",		x_left_bracket)		FLAGS(COMPILE)
    PRIM("]",		x_right_bracket)

    PRIM("C\"",		x_c_quote)		FLAGS(COMPILE)
    PRIM("HEX",		x_hex)
    PRIM("PARSE",	x_parse)
    PRIM("REFILL",	x_refill)
END_DIRECT // }


static
DIRECT_FORTH(init_file_prim) // {
    PRIM("BIN",              x_bin)
    PRIM("CLOSE-FILE",       x_close_file)
    PRIM("CREATE-FILE",      x_create_file)
    PRIM("FILE-POSITION",    x_file_position)
    PRIM("REPOSITION-FILE",  x_reposition_file)
    PRIM("FILE-SIZE",        x_file_size)
    PRIM("OPEN-FILE",        x_open_file)
    PRIM("R/O",              x_r_o)
    PRIM("R/W",              x_r_w)
    PRIM("READ-FILE",        x_read_file)
    PRIM("W/O",              x_w_o)
    PRIM("WRITE-FILE",       x_write_file)
END_DIRECT // }


DIRECT_FORTH(initialize_forth) // {
    CALL(init_dictionary)
    CALL(init_stack_prim)
    CALL(init_arith_prim)
    CALL(init_memory_prim)
    CALL(init_mult_prim)
    CALL(init_terminal_prim)
    CALL(init_names)
    CALL(init_control)
    CALL(init_interpret)
    CALL(init_file_prim)
END_DIRECT // }
