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
/* 148 */ { .handler = i_addname }, { .id = "FILL" }, { .handler = x_fill },
/* 151 */ { .handler = i_addname }, { .id = "MOVE" }, { .handler = x_move },
/* 154 */ { .handler = i_addname }, { .id = "*" }, { .handler = x_star },
/* 157 */ { .handler = i_addname }, { .id = "/" }, { .handler = x_slash },
/* 160 */ { .handler = i_addname }, { .id = "/MOD" }, { .handler = x_slash_mod },
/* 163 */ { .handler = i_addname }, { .id = "M*" }, { .handler = x_m_star },
/* 166 */ { .handler = i_addname }, { .id = "MOD" }, { .handler = x_mod },
/* 169 */ { .handler = i_addname }, { .id = "UM*" }, { .handler = x_u_m_star },
/* 172 */ { .handler = i_addname }, { .id = "UM/MOD" }, { .handler = x_u_m_slash_mod },
/* 175 */ { .handler = i_addname }, { .id = "EMIT" }, { .handler = x_emit },
/* 178 */ { .handler = do_literal }, { .cell = (cell_ft) (do_does) },
/* 180 */ { .handler = do_literal }, { .cell = (cell_ft) (DOES_XT) },
/* 182 */ { .handler = meta_interpret },
/* 183 */     { .id = "!" },
/* 184 */     { .id = NULL },
/* 185 */ { .handler = i_addname }, { .id = "'" }, { .handler = x_tick },
/* 188 */ { .handler = i_addname }, { .id = ":" }, { .handler = x_colon },
/* 191 */ { .handler = i_addname }, { .id = "EXIT" }, { .handler = x_exit },
/* 194 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 196 */ { .handler = i_addname }, { .id = ";" }, { .handler = x_semicolon },
/* 199 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 201 */ { .handler = i_compile }, { .id = "EXIT" },
/* 203 */ { .handler = i_addname }, { .id = ">BODY" }, { .handler = x_to_body },
/* 206 */ { .handler = i_addname }, { .id = "CONSTANT" }, { .handler = x_constant },
/* 209 */ { .handler = i_addname }, { .id = "CREATE" }, { .handler = x_create },
/* 212 */ { .handler = i_addname }, { .id = "DOES>" }, { .handler = x_does },
/* 215 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 217 */ { .handler = i_addname }, { .id = "FIND" }, { .handler = x_find },
/* 220 */ { .handler = i_addname }, { .id = "VARIABLE" }, { .handler = x_variable },
/* 223 */ { .handler = i_addname }, { .id = "FORTH-WORDLIST" }, { .handler = x_forth_wordlist },
/* 226 */ { .handler = i_addname }, { .id = "SEARCH-WORDLIST" }, { .handler = x_search_wordlist },
/* 229 */ { .handler = do_literal }, { .cell = (cell_ft) (do_do) },
/* 231 */ { .handler = do_literal }, { .cell = (cell_ft) (DO_DO_XT) },
/* 233 */ { .handler = meta_interpret },
/* 234 */     { .id = "!" },
/* 235 */     { .id = NULL },
/* 236 */ { .handler = do_literal }, { .cell = (cell_ft) (do_plus_loop) },
/* 238 */ { .handler = do_literal }, { .cell = (cell_ft) (PLUS_LOOP_XT) },
/* 240 */ { .handler = meta_interpret },
/* 241 */     { .id = "!" },
/* 242 */     { .id = NULL },
/* 243 */ { .handler = do_literal }, { .cell = (cell_ft) (do_skip) },
/* 245 */ { .handler = do_literal }, { .cell = (cell_ft) (SKIP_XT) },
/* 247 */ { .handler = meta_interpret },
/* 248 */     { .id = "!" },
/* 249 */     { .id = NULL },
/* 250 */ { .handler = do_literal }, { .cell = (cell_ft) (do_fskip) },
/* 252 */ { .handler = do_literal }, { .cell = (cell_ft) (FSKIP_XT) },
/* 254 */ { .handler = meta_interpret },
/* 255 */     { .id = "!" },
/* 256 */     { .id = NULL },
/* 257 */ { .handler = i_addname }, { .id = "UNLOOP" }, { .handler = x_unloop },
/* 260 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 262 */ { .handler = i_addname }, { .id = "+LOOP" }, { .handler = x_plus_loop },
/* 265 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 267 */ { .handler = i_compile }, { .id = "UNLOOP" },
/* 269 */ { .handler = i_addname }, { .id = "BEGIN" }, { .handler = x_begin },
/* 272 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 274 */ { .handler = i_addname }, { .id = "DO" }, { .handler = x_do },
/* 277 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 279 */ { .handler = i_addname }, { .id = "ELSE" }, { .handler = x_else },
/* 282 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 284 */ { .handler = i_addname }, { .id = "I" }, { .handler = x_i },
/* 287 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 289 */ { .handler = i_addname }, { .id = "IF" }, { .handler = x_if },
/* 292 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 294 */ { .handler = i_addname }, { .id = "J" }, { .handler = x_j },
/* 297 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 299 */ { .handler = i_addname }, { .id = "LEAVE" }, { .handler = c_leave },
/* 302 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 304 */ { .handler = i_addname }, { .id = "REPEAT" }, { .handler = x_repeat },
/* 307 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 309 */ { .handler = i_addname }, { .id = "THEN" }, { .handler = x_then },
/* 312 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 314 */ { .handler = i_addname }, { .id = "UNTIL" }, { .handler = x_until },
/* 317 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 319 */ { .handler = i_addname }, { .id = "WHILE" }, { .handler = x_while },
/* 322 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 324 */ { .handler = do_literal }, { .cell = (cell_ft) (do_literal) },
/* 326 */ { .handler = do_literal }, { .cell = (cell_ft) (DO_LITERAL_XT) },
/* 328 */ { .handler = meta_interpret },
/* 329 */     { .id = "!" },
/* 330 */     { .id = NULL },
/* 331 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.to_in) },
/* 333 */ { .handler = i_addname }, { .id = ">IN" }, { .handler = do_constant },
/* 336 */ /* 336 */ { .handler = x_comma },
/* 337 */ { .handler = i_addname }, { .id = "ABORT" }, { .handler = x_abort },
/* 340 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.base) },
/* 342 */ { .handler = do_literal }, { .cell = (cell_ft) (0xA) },
/* 344 */ { .handler = meta_interpret },
/* 345 */     { .id = "over" },
/* 346 */     { .id = "!" },
/* 347 */     { .id = NULL },
/* 348 */ { .handler = i_addname }, { .id = "BASE" }, { .handler = do_constant },
/* 351 */ /* 351 */ { .handler = x_comma },
/* 352 */ { .handler = i_addname }, { .id = "CHAR" }, { .handler = x_char },
/* 355 */ { .handler = i_addname }, { .id = "DECIMAL" }, { .handler = x_decimal },
/* 358 */ { .handler = i_addname }, { .id = "EVALUATE" }, { .handler = x_evaluate },
/* 361 */ { .handler = i_addname }, { .id = "EXECUTE" }, { .handler = x_execute },
/* 364 */ { .handler = i_addname }, { .id = "LITERAL" }, { .handler = x_literal },
/* 367 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 369 */ { .handler = i_addname }, { .id = "POSTPONE" }, { .handler = x_postpone },
/* 372 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 374 */ { .handler = do_literal }, { .cell = (cell_ft) (do_postpone) },
/* 376 */ { .handler = meta_interpret },
/* 377 */     { .id = "," },
/* 378 */     { .id = NULL },
/* 379 */ { .handler = i_addname }, { .id = "QUIT" }, { .handler = x_quit },
/* 382 */ { .handler = i_addname }, { .id = "SOURCE" }, { .handler = x_source },
/* 385 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.state) },
/* 387 */ { .handler = i_addname }, { .id = "STATE" }, { .handler = do_constant },
/* 390 */ /* 390 */ { .handler = x_comma },
/* 391 */ { .handler = i_addname }, { .id = "S\"" }, { .handler = x_s_quote },
/* 394 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 396 */ { .handler = do_literal }, { .cell = (cell_ft) (do_s_quote) },
/* 398 */ { .handler = meta_interpret },
/* 399 */     { .id = "," },
/* 400 */     { .id = NULL },
/* 401 */ { .handler = i_addname }, { .id = "[" }, { .handler = x_left_bracket },
/* 404 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 406 */ { .handler = i_addname }, { .id = "]" }, { .handler = x_right_bracket },
/* 409 */ { .handler = i_addname }, { .id = "C\"" }, { .handler = x_c_quote },
/* 412 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 414 */ { .handler = do_literal }, { .cell = (cell_ft) (do_c_quote) },
/* 416 */ { .handler = meta_interpret },
/* 417 */     { .id = "," },
/* 418 */     { .id = NULL },
/* 419 */ { .handler = i_addname }, { .id = "HEX" }, { .handler = x_hex },
/* 422 */ { .handler = i_addname }, { .id = "PARSE" }, { .handler = x_parse },
/* 425 */ { .handler = i_addname }, { .id = "REFILL" }, { .handler = x_refill },
/* 428 */ { .handler = i_addname }, { .id = "THROW" }, { .handler = x_throw },
/* 431 */ { .handler = i_addname }, { .id = "BIN" }, { .handler = x_bin },
/* 434 */ { .handler = i_addname }, { .id = "CLOSE-FILE" }, { .handler = x_close_file },
/* 437 */ { .handler = i_addname }, { .id = "CREATE-FILE" }, { .handler = x_create_file },
/* 440 */ { .handler = i_addname }, { .id = "FILE-POSITION" }, { .handler = x_file_position },
/* 443 */ { .handler = i_addname }, { .id = "FILE-SIZE" }, { .handler = x_file_size },
/* 446 */ { .handler = i_addname }, { .id = "OPEN-FILE" }, { .handler = x_open_file },
/* 449 */ { .handler = i_addname }, { .id = "R/O" }, { .handler = x_r_o },
/* 452 */ { .handler = i_addname }, { .id = "R/W" }, { .handler = x_r_w },
/* 455 */ { .handler = i_addname }, { .id = "READ-FILE" }, { .handler = x_read_file },
/* 458 */ { .handler = i_addname }, { .id = "REPOSITION-FILE" }, { .handler = x_reposition_file },
/* 461 */ { .handler = i_addname }, { .id = "W/O" }, { .handler = x_w_o },
/* 464 */ { .handler = i_addname }, { .id = "WRITE-FILE" }, { .handler = x_write_file },
/* 467 */ { .handler = i_startname }, { .id = "CHARS" }, { .handler = do_colon },
/* 470 */ { .handler = meta_compile },
/* 471 */     { .id = "EXIT" },
/* 472 */     { .id = NULL },
/* 473 */ { .handler = i_linkname },
/* 474 */ { .handler = i_startname }, { .id = "CHAR+" }, { .handler = do_colon },
/* 477 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/* 479 */ { .handler = meta_interpret },
/* 480 */     { .id = "LITERAL" },
/* 481 */     { .id = NULL },
/* 482 */ { .handler = meta_compile },
/* 483 */     { .id = "+" },
/* 484 */     { .id = "EXIT" },
/* 485 */     { .id = NULL },
/* 486 */ { .handler = i_linkname },
/* 487 */ { .handler = i_startname }, { .id = "CELLS" }, { .handler = do_colon },
/* 490 */ { .handler = do_literal }, { .cell = (cell_ft) (CELL_SIZE) },
/* 492 */ { .handler = meta_interpret },
/* 493 */     { .id = "LITERAL" },
/* 494 */     { .id = NULL },
/* 495 */ { .handler = meta_compile },
/* 496 */     { .id = "*" },
/* 497 */     { .id = "EXIT" },
/* 498 */     { .id = NULL },
/* 499 */ { .handler = i_linkname },
/* 500 */ { .handler = i_startname }, { .id = "CELL+" }, { .handler = do_colon },
/* 503 */ { .handler = do_literal }, { .cell = (cell_ft) (CELL_SIZE) },
/* 505 */ { .handler = meta_interpret },
/* 506 */     { .id = "LITERAL" },
/* 507 */     { .id = NULL },
/* 508 */ { .handler = meta_compile },
/* 509 */     { .id = "+" },
/* 510 */     { .id = "EXIT" },
/* 511 */     { .id = NULL },
/* 512 */ { .handler = i_linkname },
/* 513 */ { .handler = i_startname }, { .id = "+!" }, { .handler = do_colon },
/* 516 */ { .handler = meta_compile },
/* 517 */     { .id = "dup" },
/* 518 */     { .id = ">r" },
/* 519 */     { .id = "@" },
/* 520 */     { .id = "+" },
/* 521 */     { .id = "r>" },
/* 522 */     { .id = "!" },
/* 523 */     { .id = "EXIT" },
/* 524 */     { .id = NULL },
/* 525 */ { .handler = i_linkname },
/* 526 */ { .handler = i_startname }, { .id = "2!" }, { .handler = do_colon },
/* 529 */ { .handler = meta_compile },
/* 530 */     { .id = "swap" },
/* 531 */     { .id = "over" },
/* 532 */     { .id = "!" },
/* 533 */     { .id = "cell+" },
/* 534 */     { .id = "!" },
/* 535 */     { .id = "EXIT" },
/* 536 */     { .id = NULL },
/* 537 */ { .handler = i_linkname },
/* 538 */ { .handler = i_startname }, { .id = "2@" }, { .handler = do_colon },
/* 541 */ { .handler = meta_compile },
/* 542 */     { .id = "dup" },
/* 543 */     { .id = "cell+" },
/* 544 */     { .id = "@" },
/* 545 */     { .id = "swap" },
/* 546 */     { .id = "@" },
/* 547 */     { .id = "EXIT" },
/* 548 */     { .id = NULL },
/* 549 */ { .handler = i_linkname },
/* 550 */ { .handler = i_startname }, { .id = "ALIGNED" }, { .handler = do_colon },
/* 553 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/* 555 */ { .handler = meta_interpret },
/* 556 */     { .id = "cells" },
/* 557 */     { .id = NULL },
/* 558 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/* 560 */ { .handler = meta_interpret },
/* 561 */     { .id = "-" },
/* 562 */     { .id = "LITERAL" },
/* 563 */     { .id = NULL },
/* 564 */ { .handler = meta_compile },
/* 565 */     { .id = "+" },
/* 566 */     { .id = NULL },
/* 567 */ { .handler = do_literal }, { .cell = (cell_ft) (-0x1) },
/* 569 */ { .handler = meta_interpret },
/* 570 */     { .id = "cells" },
/* 571 */     { .id = "LITERAL" },
/* 572 */     { .id = NULL },
/* 573 */ { .handler = meta_compile },
/* 574 */     { .id = "and" },
/* 575 */     { .id = "EXIT" },
/* 576 */     { .id = NULL },
/* 577 */ { .handler = i_linkname },
/* 578 */ { .handler = i_startname }, { .id = "COUNT" }, { .handler = do_colon },
/* 581 */ { .handler = meta_compile },
/* 582 */     { .id = "dup" },
/* 583 */     { .id = "char+" },
/* 584 */     { .id = "swap" },
/* 585 */     { .id = "c@" },
/* 586 */     { .id = "EXIT" },
/* 587 */     { .id = NULL },
/* 588 */ { .handler = i_linkname },
/* 589 */ { .handler = i_startname }, { .id = "ERASE" }, { .handler = do_colon },
/* 592 */ { .handler = meta_compile },
/* 593 */     { .id = "CHARS" },
/* 594 */     { .id = NULL },
/* 595 */ { .handler = do_literal }, { .cell = (cell_ft) (0x0) },
/* 597 */ { .handler = meta_interpret },
/* 598 */     { .id = "LITERAL" },
/* 599 */     { .id = NULL },
/* 600 */ { .handler = meta_compile },
/* 601 */     { .id = "FILL" },
/* 602 */     { .id = "EXIT" },
/* 603 */     { .id = NULL },
/* 604 */ { .handler = i_linkname },
/* 605 */ { .handler = i_startname }, { .id = "(" }, { .handler = do_colon },
/* 608 */ { .handler = do_literal }, { .cell = (cell_ft) (0x29) },
/* 610 */ { .handler = meta_interpret },
/* 611 */     { .id = "LITERAL" },
/* 612 */     { .id = NULL },
/* 613 */ { .handler = meta_compile },
/* 614 */     { .id = "parse" },
/* 615 */     { .id = "drop" },
/* 616 */     { .id = "drop" },
/* 617 */     { .id = "EXIT" },
/* 618 */     { .id = NULL },
/* 619 */ { .handler = i_linkname },
/* 620 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x80) },
/* 622 */ { .handler = i_startname }, { .id = ".(" }, { .handler = do_colon },
/* 625 */ { .handler = do_literal }, { .cell = (cell_ft) (0x29) },
/* 627 */ { .handler = meta_interpret },
/* 628 */     { .id = "LITERAL" },
/* 629 */     { .id = NULL },
/* 630 */ { .handler = meta_compile },
/* 631 */     { .id = "parse" },
/* 632 */     { .id = "type" },
/* 633 */     { .id = "EXIT" },
/* 634 */     { .id = NULL },
/* 635 */ { .handler = i_linkname },
/* 636 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x80) },
/* 638 */ { .handler = i_startname }, { .id = "\\" }, { .handler = do_colon },
/* 641 */ { .handler = meta_compile },
/* 642 */     { .id = "source" },
/* 643 */     { .id = ">in" },
/* 644 */     { .id = "!" },
/* 645 */     { .id = "drop" },
/* 646 */     { .id = "EXIT" },
/* 647 */     { .id = NULL },
/* 648 */ { .handler = i_linkname },
/* 649 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x80) },
END_DIRECT // }
