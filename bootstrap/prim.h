/*
 * Copyright 2020, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef PRIM_H
#define PRIM_H

/*
 * Initialization and direct threaded interpretation primitives.
 */

/* vm execution primitives */
extern PRIM_HDLR(do_catch);
extern PRIM_HDLR(undo_catch);
extern PRIM_HDLR(x_execute);
extern PRIM_HDLR(x_exit);
extern PRIM_HDLR(x_throw);
extern PRIM_HDLR(x_clear);
extern PRIM_HDLR(x_rclear);
extern PRIM_HDLR(do_literal);
extern PRIM_HDLR(do_s_quote);
extern PRIM_HDLR(do_c_quote);

/* name definers interpretation primitives */
extern PRIM_HDLR(do_colon);
extern PRIM_HDLR(do_constant);
extern PRIM_HDLR(do_create);
extern PRIM_HDLR(do_variable);

/* stack primitives */
extern PRIM_HDLR(x_to_r);
extern PRIM_HDLR(x_question_dup);
extern PRIM_HDLR(x_depth);
extern PRIM_HDLR(x_drop);
extern PRIM_HDLR(x_dup);
extern PRIM_HDLR(x_over);
extern PRIM_HDLR(x_r_from);
extern PRIM_HDLR(x_r_fetch);
extern PRIM_HDLR(x_rot);
extern PRIM_HDLR(x_swap);
extern PRIM_HDLR(x_two_to_r);
extern PRIM_HDLR(x_two_r_from);
extern PRIM_HDLR(x_two_r_fetch);
extern PRIM_HDLR(x_pick);
extern PRIM_HDLR(x_roll);

/* arithmetic and logical primitives */
extern PRIM_HDLR(x_plus);
extern PRIM_HDLR(x_minus);
extern PRIM_HDLR(x_two_star);
extern PRIM_HDLR(x_two_slash);
extern PRIM_HDLR(x_less_than);
extern PRIM_HDLR(x_equals);
extern PRIM_HDLR(x_greater_than);
extern PRIM_HDLR(x_and);
extern PRIM_HDLR(x_invert);
extern PRIM_HDLR(x_lshift);
extern PRIM_HDLR(x_negate);
extern PRIM_HDLR(x_or);
extern PRIM_HDLR(x_rshift);
extern PRIM_HDLR(x_u_less);
extern PRIM_HDLR(x_xor);

/* memory primitives */
extern PRIM_HDLR(x_store);
extern PRIM_HDLR(x_fetch);
extern PRIM_HDLR(x_c_store);
extern PRIM_HDLR(x_c_fetch);
extern PRIM_HDLR(x_fill);
extern PRIM_HDLR(x_move);

/* multiple cell multiply and divide primitives */
extern PRIM_HDLR(x_star);
extern PRIM_HDLR(x_star_slash);
extern PRIM_HDLR(x_star_slash_mod);
extern PRIM_HDLR(x_slash);
extern PRIM_HDLR(x_slash_mod);
extern PRIM_HDLR(x_f_m_slash_mod);
extern PRIM_HDLR(x_m_star);
extern PRIM_HDLR(x_mod);
extern PRIM_HDLR(x_s_m_slash_rem);
extern PRIM_HDLR(x_u_m_star);
extern PRIM_HDLR(x_u_m_slash_mod);

/* terminal I/O primitives */
extern PRIM_HDLR(x_emit);
extern PRIM_HDLR(x_prompt_accept);

/* control flow primitives */
extern PRIM_HDLR(do_skip);
extern PRIM_HDLR(do_fskip);
extern PRIM_HDLR(do_plus_loop);
extern PRIM_HDLR(do_do);
extern PRIM_HDLR(x_i);
extern PRIM_HDLR(x_j);
extern PRIM_HDLR(x_unloop);

/* file I/O primitives */
extern PRIM_HDLR(x_bin);
extern PRIM_HDLR(x_close_file);
extern PRIM_HDLR(x_create_file);
extern PRIM_HDLR(x_file_position);
extern PRIM_HDLR(x_file_size);
extern PRIM_HDLR(x_open_file);
extern PRIM_HDLR(x_r_o);
extern PRIM_HDLR(x_r_w);
extern PRIM_HDLR(x_read_file);
extern PRIM_HDLR(x_read_line);
extern PRIM_HDLR(x_reposition_file);
extern PRIM_HDLR(x_w_o);
extern PRIM_HDLR(x_write_file);
extern PRIM_HDLR(x_write_line);

#endif // PRIM_H
