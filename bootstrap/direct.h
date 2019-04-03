/*
 * Copyright 2017, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef DIRECT_H
#define DIRECT_H

#include "forth.h"

/*
 * Initialization and direct threaded interpretation primitives.
 *
 * Dictionary initialization is done by interpreting direct-threaded
 * code stored in arrays.
 */

/* direct threaded name definition primitives */
extern vminstr_p i_startname(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p i_addname(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p i_setflags(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_startname(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_linkname(vminstr_p, vmstate_p, vmarg_p);

/* vm execution and meta interpretation primitives */
extern vminstr_p meta_interpret(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p meta_compile(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p i_lookup(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_colon(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_constant(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_create(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_variable(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_exit(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_execute(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_throw(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_clear(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_rclear(vminstr_p, vmstate_p, vmarg_p);

/* dictionary primitives */
extern vminstr_p x_comma(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_align(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_allot(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_c_comma(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_here(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_unused(vminstr_p, vmstate_p, vmarg_p);

/* stack primitives */
extern vminstr_p x_to_r(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_question_dup(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_depth(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_drop(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_dup(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_over(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_r_from(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_r_fetch(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_rot(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_swap(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_two_to_r(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_two_r_from(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_two_r_fetch(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_pick(vminstr_p, vmstate_p, vmarg_p);

/* arithmetic and logical primitives */
extern vminstr_p x_plus(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_minus(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_two_star(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_two_slash(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_less_than(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_equals(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_greater_than(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_and(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_invert(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_lshift(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_negate(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_or(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_rshift(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_u_less(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_xor(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_store(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_fetch(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_c_store(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_c_fetch(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_fill(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_move(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_star(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_slash(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_slash_mod(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_m_star(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_mod(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_u_m_star(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_u_m_slash_mod(vminstr_p, vmstate_p, vmarg_p);

/* terminal I/O primitives */
extern vminstr_p x_emit(vminstr_p, vmstate_p, vmarg_p);

/* name definition primitives */
extern vminstr_p x_tick(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_find(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_search_wordlist(vminstr_p, vmstate_p, vmarg_p);

/* control flow primitives */
extern vminstr_p do_skip(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_fskip(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_plus_loop(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_do(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_i(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_j(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_unloop(vminstr_p, vmstate_p, vmarg_p);

/* interpreter primitives */
extern vminstr_p x_abort(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_char(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_evaluate(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_literal(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_s_quote(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_s_quote(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_source(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_quit(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_postpone(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_postpone(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_left_bracket(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_right_bracket(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_c_quote(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_c_quote(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_parse(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_refill(vminstr_p, vmstate_p, vmarg_p);

/* file I/O primitives */
extern vminstr_p x_bin(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_close_file(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_create_file(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_file_position(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_file_size(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_open_file(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_r_o(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_r_w(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_read_file(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_reposition_file(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_w_o(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_write_file(vminstr_p, vmstate_p, vmarg_p);


#define DIRECT_FORTH(nm)	vminstr_d nm[] = {
#define END_DIRECT		X(x_exit) };
#define META_FORTH(nm)		DIRECT_FORTH(nm) META
#define END_META		DIRECT END_DIRECT

#define X(x)		{ .handler = x },
#define S(s)		{ .id = s },
#define N(n)		{ .cell = (cell_ft) (n) },
#define CALL(x)		X(i_call) { .ip = (x) },
#define L(x)		X(do_literal) N(x)
#define PRIM(nm, hdlr)	X(i_addname) S(nm) { .handler = hdlr },
#define FLAGS(f)	X(i_setflags) { .cell = NAME_TYPE_ ## f },

#define META		X(meta_interpret)
#define DIRECT		S(NULL)
#define ML(n)		DIRECT X(meta_literal) N(n)

#define XVARIABLE(nm)	DIRECT PRIM(nm, do_variable) L(CELL_SIZE) META ALLOT
#define XCONSTANT(nm)	DIRECT PRIM(nm, do_constant) META COMMA
#define XCOLON(nm)	DIRECT PRIM(nm, do_colon) META S("]")
#define XSEMICOLON	EXIT S("[")
#define XPOSTPONE	DIRECT X(meta_postpone)
#define INTERP(code)	S("[") code S("]")

#endif
