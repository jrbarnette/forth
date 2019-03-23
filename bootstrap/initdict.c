/*
 * Copyright 2019, by J. Richard Barnette. All Rights Reserved.
 */

/* This file was automatically generated by "gen-dict direct" */

#include <stddef.h>

#include "forth.h"
#include "direct.h"


DIRECT_FORTH(initialize_forth) // {
/*   0 */ { .handler = do_literal }, { .cell = (cell_ft) (sizeof (DICT)) },
/*   2 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.here) },
/*   4 */ { .handler = x_store },
/*   5 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.forth_wordlist) },
/*   7 */ { .handler = x_dup },
/*   8 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.current) },
/*  10 */ { .handler = x_store },
/*  11 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.search_order[0]) },
/*  13 */ { .handler = x_store },
/*  14 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/*  16 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.n_search_order) },
/*  18 */ { .handler = x_store },
/*  19 */ { .handler = do_literal }, { .cell = (cell_ft) (do_literal) },
/*  21 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.literal_instr) },
/*  23 */ { .handler = x_store },
/*  24 */ { .handler = do_literal }, { .cell = (cell_ft) (do_skip) },
/*  26 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.skip_instr) },
/*  28 */ { .handler = x_store },
/*  29 */ { .handler = do_literal }, { .cell = (cell_ft) (do_fskip) },
/*  31 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.fskip_instr) },
/*  33 */ { .handler = x_store },
/*  34 */ { .handler = do_literal }, { .cell = (cell_ft) (do_do) },
/*  36 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.do_instr) },
/*  38 */ { .handler = x_store },
/*  39 */ { .handler = do_literal }, { .cell = (cell_ft) (do_plus_loop) },
/*  41 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.plus_loop_instr) },
/*  43 */ { .handler = x_store },
/*  44 */ { .handler = do_literal }, { .cell = (cell_ft) (do_does) },
/*  46 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.does_instr) },
/*  48 */ { .handler = x_store },
/*  49 */ { .handler = do_literal }, { .cell = (cell_ft) (0x0) },
/*  51 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.state) },
/*  53 */ { .handler = x_store },
/*  54 */ { .handler = do_literal }, { .cell = (cell_ft) (0xA) },
/*  56 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.base) },
/*  58 */ { .handler = x_store },
/*  59 */ { .handler = i_addname }, { .id = "," }, { .handler = x_comma },
/*  62 */ { .handler = i_addname }, { .id = "ALIGN" }, { .handler = x_align },
/*  65 */ { .handler = i_addname }, { .id = "ALLOT" }, { .handler = x_allot },
/*  68 */ { .handler = i_addname }, { .id = "C," }, { .handler = x_c_comma },
/*  71 */ { .handler = i_addname }, { .id = "HERE" }, { .handler = x_here },
/*  74 */ { .handler = i_addname }, { .id = "UNUSED" }, { .handler = x_unused },
/*  77 */ { .handler = i_addname }, { .id = ">R" }, { .handler = x_to_r },
/*  80 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/*  82 */ { .handler = i_addname }, { .id = "?DUP" }, { .handler = x_question_dup },
/*  85 */ { .handler = i_addname }, { .id = "DEPTH" }, { .handler = x_depth },
/*  88 */ { .handler = i_addname }, { .id = "DROP" }, { .handler = x_drop },
/*  91 */ { .handler = i_addname }, { .id = "DUP" }, { .handler = x_dup },
/*  94 */ { .handler = i_addname }, { .id = "OVER" }, { .handler = x_over },
/*  97 */ { .handler = i_addname }, { .id = "R>" }, { .handler = x_r_from },
/* 100 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 102 */ { .handler = i_addname }, { .id = "R@" }, { .handler = x_r_fetch },
/* 105 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 107 */ { .handler = i_addname }, { .id = "ROT" }, { .handler = x_rot },
/* 110 */ { .handler = i_addname }, { .id = "SWAP" }, { .handler = x_swap },
/* 113 */ { .handler = i_addname }, { .id = "2>R" }, { .handler = x_two_to_r },
/* 116 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 118 */ { .handler = i_addname }, { .id = "2R>" }, { .handler = x_two_r_from },
/* 121 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 123 */ { .handler = i_addname }, { .id = "2R@" }, { .handler = x_two_r_fetch },
/* 126 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 128 */ { .handler = i_addname }, { .id = "PICK" }, { .handler = x_pick },
/* 131 */ { .handler = i_addname }, { .id = "+" }, { .handler = x_plus },
/* 134 */ { .handler = i_addname }, { .id = "-" }, { .handler = x_minus },
/* 137 */ { .handler = i_addname }, { .id = "2*" }, { .handler = x_two_star },
/* 140 */ { .handler = i_addname }, { .id = "2/" }, { .handler = x_two_slash },
/* 143 */ { .handler = i_addname }, { .id = "<" }, { .handler = x_less_than },
/* 146 */ { .handler = i_addname }, { .id = "=" }, { .handler = x_equals },
/* 149 */ { .handler = i_addname }, { .id = ">" }, { .handler = x_greater_than },
/* 152 */ { .handler = i_addname }, { .id = "AND" }, { .handler = x_and },
/* 155 */ { .handler = i_addname }, { .id = "INVERT" }, { .handler = x_invert },
/* 158 */ { .handler = i_addname }, { .id = "LSHIFT" }, { .handler = x_lshift },
/* 161 */ { .handler = i_addname }, { .id = "NEGATE" }, { .handler = x_negate },
/* 164 */ { .handler = i_addname }, { .id = "OR" }, { .handler = x_or },
/* 167 */ { .handler = i_addname }, { .id = "RSHIFT" }, { .handler = x_rshift },
/* 170 */ { .handler = i_addname }, { .id = "U<" }, { .handler = x_u_less },
/* 173 */ { .handler = i_addname }, { .id = "XOR" }, { .handler = x_xor },
/* 176 */ { .handler = i_addname }, { .id = "!" }, { .handler = x_store },
/* 179 */ { .handler = i_addname }, { .id = "@" }, { .handler = x_fetch },
/* 182 */ { .handler = i_addname }, { .id = "C!" }, { .handler = x_c_store },
/* 185 */ { .handler = i_addname }, { .id = "C@" }, { .handler = x_c_fetch },
/* 188 */ { .handler = i_addname }, { .id = "FILL" }, { .handler = x_fill },
/* 191 */ { .handler = i_addname }, { .id = "MOVE" }, { .handler = x_move },
/* 194 */ { .handler = i_addname }, { .id = "*" }, { .handler = x_star },
/* 197 */ { .handler = i_addname }, { .id = "/" }, { .handler = x_slash },
/* 200 */ { .handler = i_addname }, { .id = "/MOD" }, { .handler = x_slash_mod },
/* 203 */ { .handler = i_addname }, { .id = "M*" }, { .handler = x_m_star },
/* 206 */ { .handler = i_addname }, { .id = "MOD" }, { .handler = x_mod },
/* 209 */ { .handler = i_addname }, { .id = "UM*" }, { .handler = x_u_m_star },
/* 212 */ { .handler = i_addname }, { .id = "UM/MOD" }, { .handler = x_u_m_slash_mod },
/* 215 */ { .handler = i_addname }, { .id = "EMIT" }, { .handler = x_emit },
/* 218 */ { .handler = i_addname }, { .id = "'" }, { .handler = x_tick },
/* 221 */ { .handler = i_addname }, { .id = ":" }, { .handler = x_colon },
/* 224 */ { .handler = i_addname }, { .id = "EXIT" }, { .handler = x_exit },
/* 227 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 229 */ { .handler = i_addname }, { .id = ";" }, { .handler = x_semicolon },
/* 232 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 234 */ { .handler = i_compile }, { .id = "EXIT" },
/* 236 */ { .handler = i_addname }, { .id = ">BODY" }, { .handler = x_to_body },
/* 239 */ { .handler = i_addname }, { .id = "CONSTANT" }, { .handler = x_constant },
/* 242 */ { .handler = i_addname }, { .id = "CREATE" }, { .handler = x_create },
/* 245 */ { .handler = i_addname }, { .id = "DOES>" }, { .handler = x_does },
/* 248 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 250 */ { .handler = i_addname }, { .id = "FIND" }, { .handler = x_find },
/* 253 */ { .handler = i_addname }, { .id = "VARIABLE" }, { .handler = x_variable },
/* 256 */ { .handler = i_addname }, { .id = "FORTH-WORDLIST" }, { .handler = x_forth_wordlist },
/* 259 */ { .handler = i_addname }, { .id = "SEARCH-WORDLIST" }, { .handler = x_search_wordlist },
/* 262 */ { .handler = i_addname }, { .id = "UNLOOP" }, { .handler = x_unloop },
/* 265 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 267 */ { .handler = i_addname }, { .id = "+LOOP" }, { .handler = x_plus_loop },
/* 270 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 272 */ { .handler = i_compile }, { .id = "UNLOOP" },
/* 274 */ { .handler = i_addname }, { .id = "BEGIN" }, { .handler = x_begin },
/* 277 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 279 */ { .handler = i_addname }, { .id = "DO" }, { .handler = x_do },
/* 282 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 284 */ { .handler = i_addname }, { .id = "ELSE" }, { .handler = x_else },
/* 287 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 289 */ { .handler = i_addname }, { .id = "I" }, { .handler = x_i },
/* 292 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 294 */ { .handler = i_addname }, { .id = "IF" }, { .handler = x_if },
/* 297 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 299 */ { .handler = i_addname }, { .id = "J" }, { .handler = x_j },
/* 302 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 304 */ { .handler = i_addname }, { .id = "LEAVE" }, { .handler = c_leave },
/* 307 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 309 */ { .handler = i_addname }, { .id = "REPEAT" }, { .handler = x_repeat },
/* 312 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 314 */ { .handler = i_addname }, { .id = "THEN" }, { .handler = x_then },
/* 317 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 319 */ { .handler = i_addname }, { .id = "UNTIL" }, { .handler = x_until },
/* 322 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 324 */ { .handler = i_addname }, { .id = "WHILE" }, { .handler = x_while },
/* 327 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 329 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.to_in) },
/* 331 */ { .handler = i_addname }, { .id = ">IN" }, { .handler = do_constant },
/* 334 */ /* 334 */ { .handler = x_comma },
/* 335 */ { .handler = i_addname }, { .id = "ABORT" }, { .handler = x_abort },
/* 338 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.base) },
/* 340 */ { .handler = i_addname }, { .id = "BASE" }, { .handler = do_constant },
/* 343 */ /* 343 */ { .handler = x_comma },
/* 344 */ { .handler = i_addname }, { .id = "CHAR" }, { .handler = x_char },
/* 347 */ { .handler = i_addname }, { .id = "DECIMAL" }, { .handler = x_decimal },
/* 350 */ { .handler = i_addname }, { .id = "EVALUATE" }, { .handler = x_evaluate },
/* 353 */ { .handler = i_addname }, { .id = "EXECUTE" }, { .handler = x_execute },
/* 356 */ { .handler = i_addname }, { .id = "LITERAL" }, { .handler = x_literal },
/* 359 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 361 */ { .handler = i_addname }, { .id = "POSTPONE" }, { .handler = x_postpone },
/* 364 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 366 */ { .handler = do_literal }, { .cell = (cell_ft) (do_postpone) },
/* 368 */ { .handler = meta_interpret },
/* 369 */     { .id = "," },
/* 370 */     { .id = NULL },
/* 371 */ { .handler = i_addname }, { .id = "QUIT" }, { .handler = x_quit },
/* 374 */ { .handler = i_addname }, { .id = "SOURCE" }, { .handler = x_source },
/* 377 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.state) },
/* 379 */ { .handler = i_addname }, { .id = "STATE" }, { .handler = do_constant },
/* 382 */ /* 382 */ { .handler = x_comma },
/* 383 */ { .handler = i_addname }, { .id = "S\"" }, { .handler = x_s_quote },
/* 386 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 388 */ { .handler = do_literal }, { .cell = (cell_ft) (do_s_quote) },
/* 390 */ { .handler = meta_interpret },
/* 391 */     { .id = "," },
/* 392 */     { .id = NULL },
/* 393 */ { .handler = i_addname }, { .id = "[" }, { .handler = x_left_bracket },
/* 396 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 398 */ { .handler = i_addname }, { .id = "]" }, { .handler = x_right_bracket },
/* 401 */ { .handler = i_addname }, { .id = "C\"" }, { .handler = x_c_quote },
/* 404 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 406 */ { .handler = do_literal }, { .cell = (cell_ft) (do_c_quote) },
/* 408 */ { .handler = meta_interpret },
/* 409 */     { .id = "," },
/* 410 */     { .id = NULL },
/* 411 */ { .handler = i_addname }, { .id = "HEX" }, { .handler = x_hex },
/* 414 */ { .handler = i_addname }, { .id = "PARSE" }, { .handler = x_parse },
/* 417 */ { .handler = i_addname }, { .id = "REFILL" }, { .handler = x_refill },
/* 420 */ { .handler = i_addname }, { .id = "THROW" }, { .handler = x_throw },
/* 423 */ { .handler = i_addname }, { .id = "BIN" }, { .handler = x_bin },
/* 426 */ { .handler = i_addname }, { .id = "CLOSE-FILE" }, { .handler = x_close_file },
/* 429 */ { .handler = i_addname }, { .id = "CREATE-FILE" }, { .handler = x_create_file },
/* 432 */ { .handler = i_addname }, { .id = "FILE-POSITION" }, { .handler = x_file_position },
/* 435 */ { .handler = i_addname }, { .id = "FILE-SIZE" }, { .handler = x_file_size },
/* 438 */ { .handler = i_addname }, { .id = "OPEN-FILE" }, { .handler = x_open_file },
/* 441 */ { .handler = i_addname }, { .id = "R/O" }, { .handler = x_r_o },
/* 444 */ { .handler = i_addname }, { .id = "R/W" }, { .handler = x_r_w },
/* 447 */ { .handler = i_addname }, { .id = "READ-FILE" }, { .handler = x_read_file },
/* 450 */ { .handler = i_addname }, { .id = "REPOSITION-FILE" }, { .handler = x_reposition_file },
/* 453 */ { .handler = i_addname }, { .id = "W/O" }, { .handler = x_w_o },
/* 456 */ { .handler = i_addname }, { .id = "WRITE-FILE" }, { .handler = x_write_file },
/* 459 */ { .handler = i_startname }, { .id = "CHARS" }, { .handler = do_colon },
/* 462 */ { .handler = meta_compile },
/* 463 */     { .id = "EXIT" },
/* 464 */     { .id = NULL },
/* 465 */ { .handler = i_linkname },
/* 466 */ { .handler = i_startname }, { .id = "CHAR+" }, { .handler = do_colon },
/* 469 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/* 471 */ { .handler = meta_interpret },
/* 472 */     { .id = "LITERAL" },
/* 473 */     { .id = NULL },
/* 474 */ { .handler = meta_compile },
/* 475 */     { .id = "+" },
/* 476 */     { .id = "EXIT" },
/* 477 */     { .id = NULL },
/* 478 */ { .handler = i_linkname },
/* 479 */ { .handler = i_startname }, { .id = "CELLS" }, { .handler = do_colon },
/* 482 */ { .handler = do_literal }, { .cell = (cell_ft) (CELL_SIZE) },
/* 484 */ { .handler = meta_interpret },
/* 485 */     { .id = "LITERAL" },
/* 486 */     { .id = NULL },
/* 487 */ { .handler = meta_compile },
/* 488 */     { .id = "*" },
/* 489 */     { .id = "EXIT" },
/* 490 */     { .id = NULL },
/* 491 */ { .handler = i_linkname },
/* 492 */ { .handler = i_startname }, { .id = "CELL+" }, { .handler = do_colon },
/* 495 */ { .handler = do_literal }, { .cell = (cell_ft) (CELL_SIZE) },
/* 497 */ { .handler = meta_interpret },
/* 498 */     { .id = "LITERAL" },
/* 499 */     { .id = NULL },
/* 500 */ { .handler = meta_compile },
/* 501 */     { .id = "+" },
/* 502 */     { .id = "EXIT" },
/* 503 */     { .id = NULL },
/* 504 */ { .handler = i_linkname },
/* 505 */ { .handler = i_startname }, { .id = "+!" }, { .handler = do_colon },
/* 508 */ { .handler = meta_compile },
/* 509 */     { .id = "dup" },
/* 510 */     { .id = ">r" },
/* 511 */     { .id = "@" },
/* 512 */     { .id = "+" },
/* 513 */     { .id = "r>" },
/* 514 */     { .id = "!" },
/* 515 */     { .id = "EXIT" },
/* 516 */     { .id = NULL },
/* 517 */ { .handler = i_linkname },
/* 518 */ { .handler = i_startname }, { .id = "2!" }, { .handler = do_colon },
/* 521 */ { .handler = meta_compile },
/* 522 */     { .id = "swap" },
/* 523 */     { .id = "over" },
/* 524 */     { .id = "!" },
/* 525 */     { .id = "cell+" },
/* 526 */     { .id = "!" },
/* 527 */     { .id = "EXIT" },
/* 528 */     { .id = NULL },
/* 529 */ { .handler = i_linkname },
/* 530 */ { .handler = i_startname }, { .id = "2@" }, { .handler = do_colon },
/* 533 */ { .handler = meta_compile },
/* 534 */     { .id = "dup" },
/* 535 */     { .id = "cell+" },
/* 536 */     { .id = "@" },
/* 537 */     { .id = "swap" },
/* 538 */     { .id = "@" },
/* 539 */     { .id = "EXIT" },
/* 540 */     { .id = NULL },
/* 541 */ { .handler = i_linkname },
/* 542 */ { .handler = i_startname }, { .id = "ALIGNED" }, { .handler = do_colon },
/* 545 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/* 547 */ { .handler = meta_interpret },
/* 548 */     { .id = "cells" },
/* 549 */     { .id = NULL },
/* 550 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/* 552 */ { .handler = meta_interpret },
/* 553 */     { .id = "-" },
/* 554 */     { .id = "LITERAL" },
/* 555 */     { .id = NULL },
/* 556 */ { .handler = meta_compile },
/* 557 */     { .id = "+" },
/* 558 */     { .id = NULL },
/* 559 */ { .handler = do_literal }, { .cell = (cell_ft) (-0x1) },
/* 561 */ { .handler = meta_interpret },
/* 562 */     { .id = "cells" },
/* 563 */     { .id = "LITERAL" },
/* 564 */     { .id = NULL },
/* 565 */ { .handler = meta_compile },
/* 566 */     { .id = "and" },
/* 567 */     { .id = "EXIT" },
/* 568 */     { .id = NULL },
/* 569 */ { .handler = i_linkname },
/* 570 */ { .handler = i_startname }, { .id = "COUNT" }, { .handler = do_colon },
/* 573 */ { .handler = meta_compile },
/* 574 */     { .id = "dup" },
/* 575 */     { .id = "char+" },
/* 576 */     { .id = "swap" },
/* 577 */     { .id = "c@" },
/* 578 */     { .id = "EXIT" },
/* 579 */     { .id = NULL },
/* 580 */ { .handler = i_linkname },
/* 581 */ { .handler = i_startname }, { .id = "ERASE" }, { .handler = do_colon },
/* 584 */ { .handler = meta_compile },
/* 585 */     { .id = "CHARS" },
/* 586 */     { .id = NULL },
/* 587 */ { .handler = do_literal }, { .cell = (cell_ft) (0x0) },
/* 589 */ { .handler = meta_interpret },
/* 590 */     { .id = "LITERAL" },
/* 591 */     { .id = NULL },
/* 592 */ { .handler = meta_compile },
/* 593 */     { .id = "FILL" },
/* 594 */     { .id = "EXIT" },
/* 595 */     { .id = NULL },
/* 596 */ { .handler = i_linkname },
/* 597 */ { .handler = i_startname }, { .id = "(" }, { .handler = do_colon },
/* 600 */ { .handler = do_literal }, { .cell = (cell_ft) (0x29) },
/* 602 */ { .handler = meta_interpret },
/* 603 */     { .id = "LITERAL" },
/* 604 */     { .id = NULL },
/* 605 */ { .handler = meta_compile },
/* 606 */     { .id = "parse" },
/* 607 */     { .id = "drop" },
/* 608 */     { .id = "drop" },
/* 609 */     { .id = "EXIT" },
/* 610 */     { .id = NULL },
/* 611 */ { .handler = i_linkname },
/* 612 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x80) },
/* 614 */ { .handler = i_startname }, { .id = "\\" }, { .handler = do_colon },
/* 617 */ { .handler = meta_compile },
/* 618 */     { .id = "source" },
/* 619 */     { .id = ">in" },
/* 620 */     { .id = "!" },
/* 621 */     { .id = "drop" },
/* 622 */     { .id = "EXIT" },
/* 623 */     { .id = NULL },
/* 624 */ { .handler = i_linkname },
/* 625 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x80) },
END_DIRECT // }
