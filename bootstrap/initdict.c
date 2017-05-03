/*
 * Copyright 2017, by J. Richard Barnette. All Rights Reserved.
 */

/* This file was automaticaly generated by gen-dict */

#include "forth.h"
#include "direct.h"


DIRECT_FORTH(initialize_forth) // {
/*   0 */ { .handler = do_literal }, { .cell = (cell_ft) (sizeof (DICT)) },
/*   2 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.here) },
/*   4 */ { .handler = x_store },
/*   5 */ { .handler = do_literal }, { .cell = (cell_ft) (0x01) },
/*   7 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.n_search_order) },
/*   9 */ { .handler = x_store },
/*  10 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.forth_wordlist) },
/*  12 */ { .handler = x_dup },
/*  13 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.current) },
/*  15 */ { .handler = x_store },
/*  16 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.search_order[0]) },
/*  18 */ { .handler = x_store },
/*  19 */ { .handler = i_addname }, { .id = "," }, { .handler = x_comma },
/*  22 */ { .handler = i_addname }, { .id = "ALIGN" }, { .handler = x_align },
/*  25 */ { .handler = i_addname }, { .id = "ALLOT" }, { .handler = x_allot },
/*  28 */ { .handler = i_addname }, { .id = "C," }, { .handler = x_c_comma },
/*  31 */ { .handler = i_addname }, { .id = "HERE" }, { .handler = x_here },
/*  34 */ { .handler = i_addname }, { .id = "UNUSED" }, { .handler = x_unused },
/*  37 */ { .handler = i_addname }, { .id = ">R" }, { .handler = x_to_r },
/*  40 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/*  42 */ { .handler = i_addname }, { .id = "?DUP" }, { .handler = x_question_dup },
/*  45 */ { .handler = i_addname }, { .id = "DEPTH" }, { .handler = x_depth },
/*  48 */ { .handler = i_addname }, { .id = "DROP" }, { .handler = x_drop },
/*  51 */ { .handler = i_addname }, { .id = "DUP" }, { .handler = x_dup },
/*  54 */ { .handler = i_addname }, { .id = "OVER" }, { .handler = x_over },
/*  57 */ { .handler = i_addname }, { .id = "R>" }, { .handler = x_r_from },
/*  60 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/*  62 */ { .handler = i_addname }, { .id = "R@" }, { .handler = x_r_fetch },
/*  65 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/*  67 */ { .handler = i_addname }, { .id = "ROT" }, { .handler = x_rot },
/*  70 */ { .handler = i_addname }, { .id = "SWAP" }, { .handler = x_swap },
/*  73 */ { .handler = i_addname }, { .id = "2>R" }, { .handler = x_two_to_r },
/*  76 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/*  78 */ { .handler = i_addname }, { .id = "2R>" }, { .handler = x_two_r_from },
/*  81 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/*  83 */ { .handler = i_addname }, { .id = "2R@" }, { .handler = x_two_r_fetch },
/*  86 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/*  88 */ { .handler = i_addname }, { .id = "PICK" }, { .handler = x_pick },
/*  91 */ { .handler = i_addname }, { .id = "+" }, { .handler = x_plus },
/*  94 */ { .handler = i_addname }, { .id = "-" }, { .handler = x_minus },
/*  97 */ { .handler = i_addname }, { .id = "2*" }, { .handler = x_two_star },
/* 100 */ { .handler = i_addname }, { .id = "2/" }, { .handler = x_two_slash },
/* 103 */ { .handler = i_addname }, { .id = "<" }, { .handler = x_less_than },
/* 106 */ { .handler = i_addname }, { .id = "=" }, { .handler = x_equals },
/* 109 */ { .handler = i_addname }, { .id = ">" }, { .handler = x_greater_than },
/* 112 */ { .handler = i_addname }, { .id = "AND" }, { .handler = x_and },
/* 115 */ { .handler = i_addname }, { .id = "INVERT" }, { .handler = x_invert },
/* 118 */ { .handler = i_addname }, { .id = "LSHIFT" }, { .handler = x_lshift },
/* 121 */ { .handler = i_addname }, { .id = "NEGATE" }, { .handler = x_negate },
/* 124 */ { .handler = i_addname }, { .id = "OR" }, { .handler = x_or },
/* 127 */ { .handler = i_addname }, { .id = "RSHIFT" }, { .handler = x_rshift },
/* 130 */ { .handler = i_addname }, { .id = "U<" }, { .handler = x_u_less },
/* 133 */ { .handler = i_addname }, { .id = "XOR" }, { .handler = x_xor },
/* 136 */ { .handler = i_addname }, { .id = "!" }, { .handler = x_store },
/* 139 */ { .handler = i_addname }, { .id = "@" }, { .handler = x_fetch },
/* 142 */ { .handler = i_addname }, { .id = "C!" }, { .handler = x_c_store },
/* 145 */ { .handler = i_addname }, { .id = "C@" }, { .handler = x_c_fetch },
/* 148 */ { .handler = i_addname }, { .id = "MOVE" }, { .handler = x_move },
/* 151 */ { .handler = i_addname }, { .id = "*" }, { .handler = x_star },
/* 154 */ { .handler = i_addname }, { .id = "/" }, { .handler = x_slash },
/* 157 */ { .handler = i_addname }, { .id = "/MOD" }, { .handler = x_slash_mod },
/* 160 */ { .handler = i_addname }, { .id = "M*" }, { .handler = x_m_star },
/* 163 */ { .handler = i_addname }, { .id = "MOD" }, { .handler = x_mod },
/* 166 */ { .handler = i_addname }, { .id = "UM*" }, { .handler = x_u_m_star },
/* 169 */ { .handler = i_addname }, { .id = "UM/MOD" }, { .handler = x_u_m_slash_mod },
/* 172 */ { .handler = i_addname }, { .id = "EMIT" }, { .handler = x_emit },
/* 175 */ { .handler = do_literal }, { .cell = (cell_ft) (do_does) },
/* 177 */ { .handler = do_literal }, { .cell = (cell_ft) (DOES_XT) },
/* 179 */ { .handler = x_store },
/* 180 */ { .handler = i_addname }, { .id = "'" }, { .handler = x_tick },
/* 183 */ { .handler = i_addname }, { .id = ":" }, { .handler = x_colon },
/* 186 */ { .handler = i_addname }, { .id = "EXIT" }, { .handler = x_exit },
/* 189 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 191 */ { .handler = i_addname }, { .id = ";" }, { .handler = x_semicolon },
/* 194 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 196 */ { .handler = i_compile }, { .id = "EXIT" },
/* 198 */ { .handler = i_addname }, { .id = ">BODY" }, { .handler = x_to_body },
/* 201 */ { .handler = i_addname }, { .id = "CONSTANT" }, { .handler = x_constant },
/* 204 */ { .handler = i_addname }, { .id = "CREATE" }, { .handler = x_create },
/* 207 */ { .handler = i_addname }, { .id = "DOES>" }, { .handler = x_does },
/* 210 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 212 */ { .handler = i_addname }, { .id = "FIND" }, { .handler = x_find },
/* 215 */ { .handler = i_addname }, { .id = "VARIABLE" }, { .handler = x_variable },
/* 218 */ { .handler = i_addname }, { .id = "FORTH-WORDLIST" }, { .handler = x_forth_wordlist },
/* 221 */ { .handler = i_addname }, { .id = "SEARCH-WORDLIST" }, { .handler = x_search_wordlist },
/* 224 */ { .handler = do_literal }, { .cell = (cell_ft) (do_do) },
/* 226 */ { .handler = do_literal }, { .cell = (cell_ft) (DO_DO_XT) },
/* 228 */ { .handler = x_store },
/* 229 */ { .handler = do_literal }, { .cell = (cell_ft) (do_plus_loop) },
/* 231 */ { .handler = do_literal }, { .cell = (cell_ft) (PLUS_LOOP_XT) },
/* 233 */ { .handler = x_store },
/* 234 */ { .handler = do_literal }, { .cell = (cell_ft) (do_skip) },
/* 236 */ { .handler = do_literal }, { .cell = (cell_ft) (SKIP_XT) },
/* 238 */ { .handler = x_store },
/* 239 */ { .handler = do_literal }, { .cell = (cell_ft) (do_fskip) },
/* 241 */ { .handler = do_literal }, { .cell = (cell_ft) (FSKIP_XT) },
/* 243 */ { .handler = x_store },
/* 244 */ { .handler = i_addname }, { .id = "UNLOOP" }, { .handler = x_unloop },
/* 247 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 249 */ { .handler = i_addname }, { .id = "+LOOP" }, { .handler = x_plus_loop },
/* 252 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 254 */ { .handler = i_compile }, { .id = "UNLOOP" },
/* 256 */ { .handler = i_addname }, { .id = "BEGIN" }, { .handler = x_begin },
/* 259 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 261 */ { .handler = i_addname }, { .id = "DO" }, { .handler = x_do },
/* 264 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 266 */ { .handler = i_addname }, { .id = "ELSE" }, { .handler = x_else },
/* 269 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 271 */ { .handler = i_addname }, { .id = "I" }, { .handler = x_i },
/* 274 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 276 */ { .handler = i_addname }, { .id = "IF" }, { .handler = x_if },
/* 279 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 281 */ { .handler = i_addname }, { .id = "J" }, { .handler = x_j },
/* 284 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 286 */ { .handler = i_addname }, { .id = "LEAVE" }, { .handler = c_leave },
/* 289 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 291 */ { .handler = i_addname }, { .id = "REPEAT" }, { .handler = x_repeat },
/* 294 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 296 */ { .handler = i_addname }, { .id = "THEN" }, { .handler = x_then },
/* 299 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 301 */ { .handler = i_addname }, { .id = "UNTIL" }, { .handler = x_until },
/* 304 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 306 */ { .handler = i_addname }, { .id = "WHILE" }, { .handler = x_while },
/* 309 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 311 */ { .handler = do_literal }, { .cell = (cell_ft) (do_literal) },
/* 313 */ { .handler = do_literal }, { .cell = (cell_ft) (DO_LITERAL_XT) },
/* 315 */ { .handler = x_store },
/* 316 */ { .handler = do_literal }, { .cell = (cell_ft) (do_postpone) },
/* 318 */ { .handler = do_literal }, { .cell = (cell_ft) (DO_POSTPONE_XT) },
/* 320 */ { .handler = x_store },
/* 321 */ { .handler = do_literal }, { .cell = (cell_ft) (do_s_quote) },
/* 323 */ { .handler = do_literal }, { .cell = (cell_ft) (S_QUOTE_XT) },
/* 325 */ { .handler = x_store },
/* 326 */ { .handler = do_literal }, { .cell = (cell_ft) (do_c_quote) },
/* 328 */ { .handler = do_literal }, { .cell = (cell_ft) (C_QUOTE_XT) },
/* 330 */ { .handler = x_store },
/* 331 */ { .handler = do_literal }, { .cell = (cell_ft) (0x0A) },
/* 333 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.base) },
/* 335 */ { .handler = x_store },
/* 336 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.to_in) },
/* 338 */ { .handler = i_addname }, { .id = ">IN" }, { .handler = do_constant },
/* 341 */ { .handler = x_comma },
/* 342 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.base) },
/* 344 */ { .handler = i_addname }, { .id = "BASE" }, { .handler = do_constant },
/* 347 */ { .handler = x_comma },
/* 348 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.state) },
/* 350 */ { .handler = i_addname }, { .id = "STATE" }, { .handler = do_constant },
/* 353 */ { .handler = x_comma },
/* 354 */ { .handler = i_addname }, { .id = "ABORT" }, { .handler = x_abort },
/* 357 */ { .handler = i_addname }, { .id = "CHAR" }, { .handler = x_char },
/* 360 */ { .handler = i_addname }, { .id = "DECIMAL" }, { .handler = x_decimal },
/* 363 */ { .handler = i_addname }, { .id = "EVALUATE" }, { .handler = x_evaluate },
/* 366 */ { .handler = i_addname }, { .id = "EXECUTE" }, { .handler = x_execute },
/* 369 */ { .handler = i_addname }, { .id = "LITERAL" }, { .handler = x_literal },
/* 372 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 374 */ { .handler = i_addname }, { .id = "POSTPONE" }, { .handler = x_postpone },
/* 377 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 379 */ { .handler = i_addname }, { .id = "QUIT" }, { .handler = x_quit },
/* 382 */ { .handler = i_addname }, { .id = "SOURCE" }, { .handler = x_source },
/* 385 */ { .handler = i_addname }, { .id = "S\"" }, { .handler = x_s_quote },
/* 388 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 390 */ { .handler = i_addname }, { .id = "[" }, { .handler = x_left_bracket },
/* 393 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 395 */ { .handler = i_addname }, { .id = "]" }, { .handler = x_right_bracket },
/* 398 */ { .handler = i_addname }, { .id = "C\"" }, { .handler = x_c_quote },
/* 401 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 403 */ { .handler = i_addname }, { .id = "HEX" }, { .handler = x_hex },
/* 406 */ { .handler = i_addname }, { .id = "PARSE" }, { .handler = x_parse },
/* 409 */ { .handler = i_addname }, { .id = "REFILL" }, { .handler = x_refill },
/* 412 */ { .handler = i_addname }, { .id = "BIN" }, { .handler = x_bin },
/* 415 */ { .handler = i_addname }, { .id = "CLOSE-FILE" }, { .handler = x_close_file },
/* 418 */ { .handler = i_addname }, { .id = "CREATE-FILE" }, { .handler = x_create_file },
/* 421 */ { .handler = i_addname }, { .id = "FILE-POSITION" }, { .handler = x_file_position },
/* 424 */ { .handler = i_addname }, { .id = "FILE-SIZE" }, { .handler = x_file_size },
/* 427 */ { .handler = i_addname }, { .id = "OPEN-FILE" }, { .handler = x_open_file },
/* 430 */ { .handler = i_addname }, { .id = "R/O" }, { .handler = x_r_o },
/* 433 */ { .handler = i_addname }, { .id = "R/W" }, { .handler = x_r_w },
/* 436 */ { .handler = i_addname }, { .id = "READ-FILE" }, { .handler = x_read_file },
/* 439 */ { .handler = i_addname }, { .id = "REPOSITION-FILE" }, { .handler = x_reposition_file },
/* 442 */ { .handler = i_addname }, { .id = "W/O" }, { .handler = x_w_o },
/* 445 */ { .handler = i_addname }, { .id = "WRITE-FILE" }, { .handler = x_write_file },
END_DIRECT // }
