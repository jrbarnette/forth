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
/*  24 */ { .handler = do_literal }, { .cell = (cell_ft) (do_does) },
/*  26 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.does_instr) },
/*  28 */ { .handler = x_store },
/*  29 */ { .handler = do_literal }, { .cell = (cell_ft) (0x0) },
/*  31 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.state) },
/*  33 */ { .handler = x_store },
/*  34 */ { .handler = do_literal }, { .cell = (cell_ft) (0xA) },
/*  36 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.base) },
/*  38 */ { .handler = x_store },
/*  39 */ { .handler = i_addname }, { .id = "," }, { .handler = x_comma },
/*  42 */ { .handler = i_addname }, { .id = "ALIGN" }, { .handler = x_align },
/*  45 */ { .handler = i_addname }, { .id = "ALLOT" }, { .handler = x_allot },
/*  48 */ { .handler = i_addname }, { .id = "C," }, { .handler = x_c_comma },
/*  51 */ { .handler = i_addname }, { .id = "HERE" }, { .handler = x_here },
/*  54 */ { .handler = i_addname }, { .id = "UNUSED" }, { .handler = x_unused },
/*  57 */ { .handler = i_addname }, { .id = ">R" }, { .handler = x_to_r },
/*  60 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/*  62 */ { .handler = i_addname }, { .id = "?DUP" }, { .handler = x_question_dup },
/*  65 */ { .handler = i_addname }, { .id = "DEPTH" }, { .handler = x_depth },
/*  68 */ { .handler = i_addname }, { .id = "DROP" }, { .handler = x_drop },
/*  71 */ { .handler = i_addname }, { .id = "DUP" }, { .handler = x_dup },
/*  74 */ { .handler = i_addname }, { .id = "OVER" }, { .handler = x_over },
/*  77 */ { .handler = i_addname }, { .id = "R>" }, { .handler = x_r_from },
/*  80 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/*  82 */ { .handler = i_addname }, { .id = "R@" }, { .handler = x_r_fetch },
/*  85 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/*  87 */ { .handler = i_addname }, { .id = "ROT" }, { .handler = x_rot },
/*  90 */ { .handler = i_addname }, { .id = "SWAP" }, { .handler = x_swap },
/*  93 */ { .handler = i_addname }, { .id = "2>R" }, { .handler = x_two_to_r },
/*  96 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/*  98 */ { .handler = i_addname }, { .id = "2R>" }, { .handler = x_two_r_from },
/* 101 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 103 */ { .handler = i_addname }, { .id = "2R@" }, { .handler = x_two_r_fetch },
/* 106 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 108 */ { .handler = i_addname }, { .id = "PICK" }, { .handler = x_pick },
/* 111 */ { .handler = i_addname }, { .id = "+" }, { .handler = x_plus },
/* 114 */ { .handler = i_addname }, { .id = "-" }, { .handler = x_minus },
/* 117 */ { .handler = i_addname }, { .id = "2*" }, { .handler = x_two_star },
/* 120 */ { .handler = i_addname }, { .id = "2/" }, { .handler = x_two_slash },
/* 123 */ { .handler = i_addname }, { .id = "<" }, { .handler = x_less_than },
/* 126 */ { .handler = i_addname }, { .id = "=" }, { .handler = x_equals },
/* 129 */ { .handler = i_addname }, { .id = ">" }, { .handler = x_greater_than },
/* 132 */ { .handler = i_addname }, { .id = "AND" }, { .handler = x_and },
/* 135 */ { .handler = i_addname }, { .id = "INVERT" }, { .handler = x_invert },
/* 138 */ { .handler = i_addname }, { .id = "LSHIFT" }, { .handler = x_lshift },
/* 141 */ { .handler = i_addname }, { .id = "NEGATE" }, { .handler = x_negate },
/* 144 */ { .handler = i_addname }, { .id = "OR" }, { .handler = x_or },
/* 147 */ { .handler = i_addname }, { .id = "RSHIFT" }, { .handler = x_rshift },
/* 150 */ { .handler = i_addname }, { .id = "U<" }, { .handler = x_u_less },
/* 153 */ { .handler = i_addname }, { .id = "XOR" }, { .handler = x_xor },
/* 156 */ { .handler = i_addname }, { .id = "!" }, { .handler = x_store },
/* 159 */ { .handler = i_addname }, { .id = "@" }, { .handler = x_fetch },
/* 162 */ { .handler = i_addname }, { .id = "C!" }, { .handler = x_c_store },
/* 165 */ { .handler = i_addname }, { .id = "C@" }, { .handler = x_c_fetch },
/* 168 */ { .handler = i_addname }, { .id = "FILL" }, { .handler = x_fill },
/* 171 */ { .handler = i_addname }, { .id = "MOVE" }, { .handler = x_move },
/* 174 */ { .handler = i_addname }, { .id = "*" }, { .handler = x_star },
/* 177 */ { .handler = i_addname }, { .id = "/" }, { .handler = x_slash },
/* 180 */ { .handler = i_addname }, { .id = "/MOD" }, { .handler = x_slash_mod },
/* 183 */ { .handler = i_addname }, { .id = "M*" }, { .handler = x_m_star },
/* 186 */ { .handler = i_addname }, { .id = "MOD" }, { .handler = x_mod },
/* 189 */ { .handler = i_addname }, { .id = "UM*" }, { .handler = x_u_m_star },
/* 192 */ { .handler = i_addname }, { .id = "UM/MOD" }, { .handler = x_u_m_slash_mod },
/* 195 */ { .handler = i_addname }, { .id = "EMIT" }, { .handler = x_emit },
/* 198 */ { .handler = i_addname }, { .id = "'" }, { .handler = x_tick },
/* 201 */ { .handler = i_addname }, { .id = ":" }, { .handler = x_colon },
/* 204 */ { .handler = i_addname }, { .id = "EXIT" }, { .handler = x_exit },
/* 207 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 209 */ { .handler = i_addname }, { .id = ";" }, { .handler = x_semicolon },
/* 212 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 214 */ { .handler = meta_compile },
/* 215 */     { .id = "EXIT" },
/* 216 */     { .id = NULL },
/* 217 */ { .handler = i_addname }, { .id = ">BODY" }, { .handler = x_to_body },
/* 220 */ { .handler = i_addname }, { .id = "CONSTANT" }, { .handler = x_constant },
/* 223 */ { .handler = i_addname }, { .id = "CREATE" }, { .handler = x_create },
/* 226 */ { .handler = i_addname }, { .id = "DOES>" }, { .handler = x_does },
/* 229 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 231 */ { .handler = i_addname }, { .id = "FIND" }, { .handler = x_find },
/* 234 */ { .handler = i_addname }, { .id = "VARIABLE" }, { .handler = x_variable },
/* 237 */ { .handler = i_addname }, { .id = "FORTH-WORDLIST" }, { .handler = x_forth_wordlist },
/* 240 */ { .handler = i_addname }, { .id = "SEARCH-WORDLIST" }, { .handler = x_search_wordlist },
/* 243 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.to_in) },
/* 245 */ { .handler = i_addname }, { .id = ">IN" }, { .handler = do_constant },
/* 248 */ { .handler = x_comma },
/* 249 */ { .handler = i_addname }, { .id = "ABORT" }, { .handler = x_abort },
/* 252 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.base) },
/* 254 */ { .handler = i_addname }, { .id = "BASE" }, { .handler = do_constant },
/* 257 */ { .handler = x_comma },
/* 258 */ { .handler = i_addname }, { .id = "CHAR" }, { .handler = x_char },
/* 261 */ { .handler = i_addname }, { .id = "DECIMAL" }, { .handler = x_decimal },
/* 264 */ { .handler = i_addname }, { .id = "EVALUATE" }, { .handler = x_evaluate },
/* 267 */ { .handler = i_addname }, { .id = "EXECUTE" }, { .handler = x_execute },
/* 270 */ { .handler = i_addname }, { .id = "LITERAL" }, { .handler = x_literal },
/* 273 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 275 */ { .handler = i_addname }, { .id = "POSTPONE" }, { .handler = x_postpone },
/* 278 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 280 */ { .handler = do_literal }, { .cell = (cell_ft) (do_postpone) },
/* 282 */ { .handler = meta_interpret },
/* 283 */     { .id = "," },
/* 284 */     { .id = NULL },
/* 285 */ { .handler = i_addname }, { .id = "QUIT" }, { .handler = x_quit },
/* 288 */ { .handler = i_addname }, { .id = "SOURCE" }, { .handler = x_source },
/* 291 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.state) },
/* 293 */ { .handler = i_addname }, { .id = "STATE" }, { .handler = do_constant },
/* 296 */ { .handler = x_comma },
/* 297 */ { .handler = i_addname }, { .id = "S\"" }, { .handler = x_s_quote },
/* 300 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 302 */ { .handler = do_literal }, { .cell = (cell_ft) (do_s_quote) },
/* 304 */ { .handler = meta_interpret },
/* 305 */     { .id = "," },
/* 306 */     { .id = NULL },
/* 307 */ { .handler = i_addname }, { .id = "[" }, { .handler = x_left_bracket },
/* 310 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 312 */ { .handler = i_addname }, { .id = "]" }, { .handler = x_right_bracket },
/* 315 */ { .handler = i_addname }, { .id = "C\"" }, { .handler = x_c_quote },
/* 318 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 320 */ { .handler = do_literal }, { .cell = (cell_ft) (do_c_quote) },
/* 322 */ { .handler = meta_interpret },
/* 323 */     { .id = "," },
/* 324 */     { .id = NULL },
/* 325 */ { .handler = i_addname }, { .id = "HEX" }, { .handler = x_hex },
/* 328 */ { .handler = i_addname }, { .id = "PARSE" }, { .handler = x_parse },
/* 331 */ { .handler = i_addname }, { .id = "REFILL" }, { .handler = x_refill },
/* 334 */ { .handler = i_addname }, { .id = "THROW" }, { .handler = x_throw },
/* 337 */ { .handler = i_addname }, { .id = "BIN" }, { .handler = x_bin },
/* 340 */ { .handler = i_addname }, { .id = "CLOSE-FILE" }, { .handler = x_close_file },
/* 343 */ { .handler = i_addname }, { .id = "CREATE-FILE" }, { .handler = x_create_file },
/* 346 */ { .handler = i_addname }, { .id = "FILE-POSITION" }, { .handler = x_file_position },
/* 349 */ { .handler = i_addname }, { .id = "FILE-SIZE" }, { .handler = x_file_size },
/* 352 */ { .handler = i_addname }, { .id = "OPEN-FILE" }, { .handler = x_open_file },
/* 355 */ { .handler = i_addname }, { .id = "R/O" }, { .handler = x_r_o },
/* 358 */ { .handler = i_addname }, { .id = "R/W" }, { .handler = x_r_w },
/* 361 */ { .handler = i_addname }, { .id = "READ-FILE" }, { .handler = x_read_file },
/* 364 */ { .handler = i_addname }, { .id = "REPOSITION-FILE" }, { .handler = x_reposition_file },
/* 367 */ { .handler = i_addname }, { .id = "W/O" }, { .handler = x_w_o },
/* 370 */ { .handler = i_addname }, { .id = "WRITE-FILE" }, { .handler = x_write_file },
/* 373 */ { .handler = i_startname }, { .id = "CHARS" }, { .handler = do_colon },
/* 376 */ { .handler = meta_compile },
/* 377 */     { .id = "EXIT" },
/* 378 */     { .id = NULL },
/* 379 */ { .handler = i_linkname },
/* 380 */ { .handler = i_startname }, { .id = "CHAR+" }, { .handler = do_colon },
/* 383 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/* 385 */ { .handler = meta_interpret },
/* 386 */     { .id = "LITERAL" },
/* 387 */     { .id = NULL },
/* 388 */ { .handler = meta_compile },
/* 389 */     { .id = "+" },
/* 390 */     { .id = "EXIT" },
/* 391 */     { .id = NULL },
/* 392 */ { .handler = i_linkname },
/* 393 */ { .handler = i_startname }, { .id = "CELLS" }, { .handler = do_colon },
/* 396 */ { .handler = do_literal }, { .cell = (cell_ft) (CELL_SIZE) },
/* 398 */ { .handler = meta_interpret },
/* 399 */     { .id = "LITERAL" },
/* 400 */     { .id = NULL },
/* 401 */ { .handler = meta_compile },
/* 402 */     { .id = "*" },
/* 403 */     { .id = "EXIT" },
/* 404 */     { .id = NULL },
/* 405 */ { .handler = i_linkname },
/* 406 */ { .handler = i_startname }, { .id = "CELL+" }, { .handler = do_colon },
/* 409 */ { .handler = do_literal }, { .cell = (cell_ft) (CELL_SIZE) },
/* 411 */ { .handler = meta_interpret },
/* 412 */     { .id = "LITERAL" },
/* 413 */     { .id = NULL },
/* 414 */ { .handler = meta_compile },
/* 415 */     { .id = "+" },
/* 416 */     { .id = "EXIT" },
/* 417 */     { .id = NULL },
/* 418 */ { .handler = i_linkname },
/* 419 */ { .handler = i_startname }, { .id = "<MARK" }, { .handler = do_colon },
/* 422 */ { .handler = meta_compile },
/* 423 */     { .id = "here" },
/* 424 */     { .id = "EXIT" },
/* 425 */     { .id = NULL },
/* 426 */ { .handler = i_linkname },
/* 427 */ { .handler = i_startname }, { .id = "<RESOLVE" }, { .handler = do_colon },
/* 430 */ { .handler = meta_compile },
/* 431 */     { .id = "here" },
/* 432 */     { .id = "-" },
/* 433 */     { .id = NULL },
/* 434 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/* 436 */ { .handler = meta_interpret },
/* 437 */     { .id = "cells" },
/* 438 */     { .id = "LITERAL" },
/* 439 */     { .id = NULL },
/* 440 */ { .handler = meta_compile },
/* 441 */     { .id = "/" },
/* 442 */     { .id = "," },
/* 443 */     { .id = "EXIT" },
/* 444 */     { .id = NULL },
/* 445 */ { .handler = i_linkname },
/* 446 */ { .handler = i_startname }, { .id = ">MARK" }, { .handler = do_colon },
/* 449 */ { .handler = meta_compile },
/* 450 */     { .id = "here" },
/* 451 */     { .id = NULL },
/* 452 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/* 454 */ { .handler = meta_interpret },
/* 455 */     { .id = "cells" },
/* 456 */     { .id = "LITERAL" },
/* 457 */     { .id = NULL },
/* 458 */ { .handler = meta_compile },
/* 459 */     { .id = "allot" },
/* 460 */     { .id = "EXIT" },
/* 461 */     { .id = NULL },
/* 462 */ { .handler = i_linkname },
/* 463 */ { .handler = i_startname }, { .id = ">RESOLVE" }, { .handler = do_colon },
/* 466 */ { .handler = meta_compile },
/* 467 */     { .id = "here" },
/* 468 */     { .id = "over" },
/* 469 */     { .id = "-" },
/* 470 */     { .id = NULL },
/* 471 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/* 473 */ { .handler = meta_interpret },
/* 474 */     { .id = "cells" },
/* 475 */     { .id = "LITERAL" },
/* 476 */     { .id = NULL },
/* 477 */ { .handler = meta_compile },
/* 478 */     { .id = "/" },
/* 479 */     { .id = "swap" },
/* 480 */     { .id = "!" },
/* 481 */     { .id = "EXIT" },
/* 482 */     { .id = NULL },
/* 483 */ { .handler = i_linkname },
/* 484 */ { .handler = meta_interpret },
/* 485 */     { .id = "here" },
/* 486 */     { .id = NULL },
/* 487 */ { .handler = do_literal }, { .cell = (cell_ft) (do_skip) },
/* 489 */ { .handler = meta_interpret },
/* 490 */     { .id = "," },
/* 491 */     { .id = NULL },
/* 492 */ { .handler = i_startname }, { .id = ">BRANCH" }, { .handler = do_colon },
/* 495 */ { .handler = meta_interpret },
/* 496 */     { .id = "over" },
/* 497 */     { .id = "LITERAL" },
/* 498 */     { .id = NULL },
/* 499 */ { .handler = meta_compile },
/* 500 */     { .id = "," },
/* 501 */     { .id = ">mark" },
/* 502 */     { .id = "EXIT" },
/* 503 */     { .id = NULL },
/* 504 */ { .handler = i_linkname },
/* 505 */ { .handler = i_startname }, { .id = "<BRANCH" }, { .handler = do_colon },
/* 508 */ { .handler = meta_interpret },
/* 509 */     { .id = "swap" },
/* 510 */     { .id = "LITERAL" },
/* 511 */     { .id = NULL },
/* 512 */ { .handler = meta_compile },
/* 513 */     { .id = "," },
/* 514 */     { .id = "<resolve" },
/* 515 */     { .id = "EXIT" },
/* 516 */     { .id = NULL },
/* 517 */ { .handler = i_linkname },
/* 518 */ { .handler = meta_interpret },
/* 519 */     { .id = "here" },
/* 520 */     { .id = NULL },
/* 521 */ { .handler = do_literal }, { .cell = (cell_ft) (do_fskip) },
/* 523 */ { .handler = meta_interpret },
/* 524 */     { .id = "," },
/* 525 */     { .id = NULL },
/* 526 */ { .handler = i_startname }, { .id = ">?BRANCH" }, { .handler = do_colon },
/* 529 */ { .handler = meta_interpret },
/* 530 */     { .id = "over" },
/* 531 */     { .id = "LITERAL" },
/* 532 */     { .id = NULL },
/* 533 */ { .handler = meta_compile },
/* 534 */     { .id = "," },
/* 535 */     { .id = ">mark" },
/* 536 */     { .id = "EXIT" },
/* 537 */     { .id = NULL },
/* 538 */ { .handler = i_linkname },
/* 539 */ { .handler = i_startname }, { .id = "<?BRANCH" }, { .handler = do_colon },
/* 542 */ { .handler = meta_interpret },
/* 543 */     { .id = "swap" },
/* 544 */     { .id = "LITERAL" },
/* 545 */     { .id = NULL },
/* 546 */ { .handler = meta_compile },
/* 547 */     { .id = "," },
/* 548 */     { .id = "<resolve" },
/* 549 */     { .id = "EXIT" },
/* 550 */     { .id = NULL },
/* 551 */ { .handler = i_linkname },
/* 552 */ { .handler = i_startname }, { .id = "BEGIN" }, { .handler = do_colon },
/* 555 */ { .handler = meta_compile },
/* 556 */     { .id = "<mark" },
/* 557 */     { .id = "EXIT" },
/* 558 */     { .id = NULL },
/* 559 */ { .handler = i_linkname },
/* 560 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 562 */ { .handler = i_startname }, { .id = "THEN" }, { .handler = do_colon },
/* 565 */ { .handler = meta_compile },
/* 566 */     { .id = ">resolve" },
/* 567 */     { .id = "EXIT" },
/* 568 */     { .id = NULL },
/* 569 */ { .handler = i_linkname },
/* 570 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 572 */ { .handler = i_startname }, { .id = "IF" }, { .handler = do_colon },
/* 575 */ { .handler = meta_compile },
/* 576 */     { .id = ">?branch" },
/* 577 */     { .id = "EXIT" },
/* 578 */     { .id = NULL },
/* 579 */ { .handler = i_linkname },
/* 580 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 582 */ { .handler = i_startname }, { .id = "UNTIL" }, { .handler = do_colon },
/* 585 */ { .handler = meta_compile },
/* 586 */     { .id = "<?branch" },
/* 587 */     { .id = "EXIT" },
/* 588 */     { .id = NULL },
/* 589 */ { .handler = i_linkname },
/* 590 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 592 */ { .handler = i_startname }, { .id = "REPEAT" }, { .handler = do_colon },
/* 595 */ { .handler = meta_compile },
/* 596 */     { .id = "<branch" },
/* 597 */     { .id = ">resolve" },
/* 598 */     { .id = "EXIT" },
/* 599 */     { .id = NULL },
/* 600 */ { .handler = i_linkname },
/* 601 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 603 */ { .handler = i_startname }, { .id = "ELSE" }, { .handler = do_colon },
/* 606 */ { .handler = meta_compile },
/* 607 */     { .id = ">branch" },
/* 608 */     { .id = "swap" },
/* 609 */     { .id = ">resolve" },
/* 610 */     { .id = "EXIT" },
/* 611 */     { .id = NULL },
/* 612 */ { .handler = i_linkname },
/* 613 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 615 */ { .handler = i_startname }, { .id = "WHILE" }, { .handler = do_colon },
/* 618 */ { .handler = meta_compile },
/* 619 */     { .id = ">?branch" },
/* 620 */     { .id = "swap" },
/* 621 */     { .id = "EXIT" },
/* 622 */     { .id = NULL },
/* 623 */ { .handler = i_linkname },
/* 624 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 626 */ { .handler = i_addname }, { .id = "LEAVERS" }, { .handler = do_variable },
/* 629 */ { .handler = do_literal }, { .cell = (cell_ft) (CELL_SIZE) },
/* 631 */ { .handler = x_allot },
/* 632 */ { .handler = do_literal }, { .cell = (cell_ft) (0x0) },
/* 634 */ { .handler = meta_interpret },
/* 635 */     { .id = "leavers" },
/* 636 */     { .id = "!" },
/* 637 */     { .id = "here" },
/* 638 */     { .id = NULL },
/* 639 */ { .handler = do_literal }, { .cell = (cell_ft) (do_do) },
/* 641 */ { .handler = meta_interpret },
/* 642 */     { .id = "," },
/* 643 */     { .id = NULL },
/* 644 */ { .handler = i_startname }, { .id = "DO" }, { .handler = do_colon },
/* 647 */ { .handler = meta_interpret },
/* 648 */     { .id = "swap" },
/* 649 */     { .id = "LITERAL" },
/* 650 */     { .id = NULL },
/* 651 */ { .handler = meta_compile },
/* 652 */     { .id = "," },
/* 653 */     { .id = "leavers" },
/* 654 */     { .id = "@" },
/* 655 */     { .id = NULL },
/* 656 */ { .handler = do_literal }, { .cell = (cell_ft) (0x0) },
/* 658 */ { .handler = meta_interpret },
/* 659 */     { .id = "LITERAL" },
/* 660 */     { .id = NULL },
/* 661 */ { .handler = meta_compile },
/* 662 */     { .id = "leavers" },
/* 663 */     { .id = "!" },
/* 664 */     { .id = "<mark" },
/* 665 */     { .id = "EXIT" },
/* 666 */     { .id = NULL },
/* 667 */ { .handler = i_linkname },
/* 668 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 670 */ { .handler = i_startname }, { .id = "LEAVE" }, { .handler = do_colon },
/* 673 */ { .handler = meta_compile },
/* 674 */     { .id = ">branch" },
/* 675 */     { .id = "leavers" },
/* 676 */     { .id = "@" },
/* 677 */     { .id = "over" },
/* 678 */     { .id = "!" },
/* 679 */     { .id = "leavers" },
/* 680 */     { .id = "!" },
/* 681 */     { .id = "EXIT" },
/* 682 */     { .id = NULL },
/* 683 */ { .handler = i_linkname },
/* 684 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 686 */ { .handler = i_addname }, { .id = "UNLOOP" }, { .handler = x_unloop },
/* 689 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 691 */ { .handler = meta_interpret },
/* 692 */     { .id = "here" },
/* 693 */     { .id = NULL },
/* 694 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/* 696 */ { .handler = meta_interpret },
/* 697 */     { .id = "cells" },
/* 698 */     { .id = "-" },
/* 699 */     { .id = NULL },
/* 700 */ { .handler = i_addname }, { .id = "I" }, { .handler = x_i },
/* 703 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 705 */ { .handler = i_addname }, { .id = "J" }, { .handler = x_j },
/* 708 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 710 */ { .handler = meta_interpret },
/* 711 */     { .id = "here" },
/* 712 */     { .id = NULL },
/* 713 */ { .handler = do_literal }, { .cell = (cell_ft) (do_plus_loop) },
/* 715 */ { .handler = meta_interpret },
/* 716 */     { .id = "," },
/* 717 */     { .id = NULL },
/* 718 */ { .handler = i_startname }, { .id = "+LOOP" }, { .handler = do_colon },
/* 721 */ { .handler = meta_interpret },
/* 722 */     { .id = "swap" },
/* 723 */     { .id = "LITERAL" },
/* 724 */     { .id = NULL },
/* 725 */ { .handler = meta_compile },
/* 726 */     { .id = "," },
/* 727 */     { .id = "<?branch" },
/* 728 */     { .id = "leavers" },
/* 729 */     { .id = "@" },
/* 730 */     { .id = "swap" },
/* 731 */     { .id = "leavers" },
/* 732 */     { .id = "!" },
/* 733 */     { .id = NULL },
/* 734 */ { .handler = meta_interpret },
/* 735 */     { .id = "BEGIN" },
/* 736 */     { .id = NULL },
/* 737 */ { .handler = meta_compile },
/* 738 */     { .id = "dup" },
/* 739 */     { .id = NULL },
/* 740 */ { .handler = meta_interpret },
/* 741 */     { .id = "WHILE" },
/* 742 */     { .id = NULL },
/* 743 */ { .handler = meta_compile },
/* 744 */     { .id = "dup" },
/* 745 */     { .id = ">resolve" },
/* 746 */     { .id = "@" },
/* 747 */     { .id = NULL },
/* 748 */ { .handler = meta_interpret },
/* 749 */     { .id = "REPEAT" },
/* 750 */     { .id = NULL },
/* 751 */ { .handler = meta_compile },
/* 752 */     { .id = "drop" },
/* 753 */     { .id = NULL },
/* 754 */ { .handler = meta_interpret },
/* 755 */     { .id = "swap" },
/* 756 */     { .id = "LITERAL" },
/* 757 */     { .id = NULL },
/* 758 */ { .handler = meta_compile },
/* 759 */     { .id = "," },
/* 760 */     { .id = "EXIT" },
/* 761 */     { .id = NULL },
/* 762 */ { .handler = i_linkname },
/* 763 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 765 */ { .handler = i_startname }, { .id = "+!" }, { .handler = do_colon },
/* 768 */ { .handler = meta_compile },
/* 769 */     { .id = "dup" },
/* 770 */     { .id = ">r" },
/* 771 */     { .id = "@" },
/* 772 */     { .id = "+" },
/* 773 */     { .id = "r>" },
/* 774 */     { .id = "!" },
/* 775 */     { .id = "EXIT" },
/* 776 */     { .id = NULL },
/* 777 */ { .handler = i_linkname },
/* 778 */ { .handler = i_startname }, { .id = "2!" }, { .handler = do_colon },
/* 781 */ { .handler = meta_compile },
/* 782 */     { .id = "swap" },
/* 783 */     { .id = "over" },
/* 784 */     { .id = "!" },
/* 785 */     { .id = "cell+" },
/* 786 */     { .id = "!" },
/* 787 */     { .id = "EXIT" },
/* 788 */     { .id = NULL },
/* 789 */ { .handler = i_linkname },
/* 790 */ { .handler = i_startname }, { .id = "2@" }, { .handler = do_colon },
/* 793 */ { .handler = meta_compile },
/* 794 */     { .id = "dup" },
/* 795 */     { .id = "cell+" },
/* 796 */     { .id = "@" },
/* 797 */     { .id = "swap" },
/* 798 */     { .id = "@" },
/* 799 */     { .id = "EXIT" },
/* 800 */     { .id = NULL },
/* 801 */ { .handler = i_linkname },
/* 802 */ { .handler = i_startname }, { .id = "ALIGNED" }, { .handler = do_colon },
/* 805 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/* 807 */ { .handler = meta_interpret },
/* 808 */     { .id = "cells" },
/* 809 */     { .id = NULL },
/* 810 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/* 812 */ { .handler = meta_interpret },
/* 813 */     { .id = "-" },
/* 814 */     { .id = "LITERAL" },
/* 815 */     { .id = NULL },
/* 816 */ { .handler = meta_compile },
/* 817 */     { .id = "+" },
/* 818 */     { .id = NULL },
/* 819 */ { .handler = do_literal }, { .cell = (cell_ft) (-0x1) },
/* 821 */ { .handler = meta_interpret },
/* 822 */     { .id = "cells" },
/* 823 */     { .id = "LITERAL" },
/* 824 */     { .id = NULL },
/* 825 */ { .handler = meta_compile },
/* 826 */     { .id = "and" },
/* 827 */     { .id = "EXIT" },
/* 828 */     { .id = NULL },
/* 829 */ { .handler = i_linkname },
/* 830 */ { .handler = i_startname }, { .id = "COUNT" }, { .handler = do_colon },
/* 833 */ { .handler = meta_compile },
/* 834 */     { .id = "dup" },
/* 835 */     { .id = "char+" },
/* 836 */     { .id = "swap" },
/* 837 */     { .id = "c@" },
/* 838 */     { .id = "EXIT" },
/* 839 */     { .id = NULL },
/* 840 */ { .handler = i_linkname },
/* 841 */ { .handler = i_startname }, { .id = "ERASE" }, { .handler = do_colon },
/* 844 */ { .handler = meta_compile },
/* 845 */     { .id = "CHARS" },
/* 846 */     { .id = NULL },
/* 847 */ { .handler = do_literal }, { .cell = (cell_ft) (0x0) },
/* 849 */ { .handler = meta_interpret },
/* 850 */     { .id = "LITERAL" },
/* 851 */     { .id = NULL },
/* 852 */ { .handler = meta_compile },
/* 853 */     { .id = "FILL" },
/* 854 */     { .id = "EXIT" },
/* 855 */     { .id = NULL },
/* 856 */ { .handler = i_linkname },
/* 857 */ { .handler = i_startname }, { .id = "(" }, { .handler = do_colon },
/* 860 */ { .handler = do_literal }, { .cell = (cell_ft) (0x29) },
/* 862 */ { .handler = meta_interpret },
/* 863 */     { .id = "LITERAL" },
/* 864 */     { .id = NULL },
/* 865 */ { .handler = meta_compile },
/* 866 */     { .id = "parse" },
/* 867 */     { .id = "drop" },
/* 868 */     { .id = "drop" },
/* 869 */     { .id = "EXIT" },
/* 870 */     { .id = NULL },
/* 871 */ { .handler = i_linkname },
/* 872 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x80) },
/* 874 */ { .handler = i_startname }, { .id = "\\" }, { .handler = do_colon },
/* 877 */ { .handler = meta_compile },
/* 878 */     { .id = "source" },
/* 879 */     { .id = ">in" },
/* 880 */     { .id = "!" },
/* 881 */     { .id = "drop" },
/* 882 */     { .id = "EXIT" },
/* 883 */     { .id = NULL },
/* 884 */ { .handler = i_linkname },
/* 885 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x80) },
END_DIRECT // }
