/*
 * Copyright 2020, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef PRIM_H
#define PRIM_H

/*
 * Initialization and direct threaded interpretation primitives.
 */

/* vm execution primitives */
extern vminstr_p do_catch(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p undo_catch(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_execute(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_exit(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_throw(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_clear(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_rclear(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_literal(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_s_quote(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_c_quote(vminstr_p, vmstate_p, vmarg_p);

/* name definers interpretation primitives */
extern vminstr_p do_colon(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_constant(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_create(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_variable(vminstr_p, vmstate_p, vmarg_p);

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
extern vminstr_p x_roll(vminstr_p, vmstate_p, vmarg_p);

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

/* memory primitives */
extern vminstr_p x_store(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_fetch(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_c_store(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_c_fetch(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_fill(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_move(vminstr_p, vmstate_p, vmarg_p);

/* multiple cell multiply and divide primitives */
extern vminstr_p x_star(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_star_slash(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_star_slash_mod(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_slash(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_slash_mod(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_f_m_slash_mod(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_m_star(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_mod(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_s_m_slash_rem(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_u_m_star(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_u_m_slash_mod(vminstr_p, vmstate_p, vmarg_p);

/* terminal I/O primitives */
extern vminstr_p x_emit(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_prompt_accept(vminstr_p, vmstate_p, vmarg_p);

/* control flow primitives */
extern vminstr_p do_skip(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_fskip(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_plus_loop(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p do_do(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_i(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_j(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_unloop(vminstr_p, vmstate_p, vmarg_p);

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
extern vminstr_p x_read_line(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_reposition_file(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_w_o(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_write_file(vminstr_p, vmstate_p, vmarg_p);
extern vminstr_p x_write_line(vminstr_p, vmstate_p, vmarg_p);

#endif // PRIM_H
