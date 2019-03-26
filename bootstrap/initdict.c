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
/*  24 */ { .handler = do_literal }, { .cell = (cell_ft) (0x0) },
/*  26 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.state) },
/*  28 */ { .handler = x_store },
/*  29 */ { .handler = do_literal }, { .cell = (cell_ft) (0xA) },
/*  31 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.base) },
/*  33 */ { .handler = x_store },
/*  34 */ { .handler = i_addname }, { .id = "," }, { .handler = x_comma },
/*  37 */ { .handler = i_addname }, { .id = "ALIGN" }, { .handler = x_align },
/*  40 */ { .handler = i_addname }, { .id = "ALLOT" }, { .handler = x_allot },
/*  43 */ { .handler = i_addname }, { .id = "C," }, { .handler = x_c_comma },
/*  46 */ { .handler = i_addname }, { .id = "HERE" }, { .handler = x_here },
/*  49 */ { .handler = i_addname }, { .id = "UNUSED" }, { .handler = x_unused },
/*  52 */ { .handler = i_addname }, { .id = ">R" }, { .handler = x_to_r },
/*  55 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/*  57 */ { .handler = i_addname }, { .id = "?DUP" }, { .handler = x_question_dup },
/*  60 */ { .handler = i_addname }, { .id = "DEPTH" }, { .handler = x_depth },
/*  63 */ { .handler = i_addname }, { .id = "DROP" }, { .handler = x_drop },
/*  66 */ { .handler = i_addname }, { .id = "DUP" }, { .handler = x_dup },
/*  69 */ { .handler = i_addname }, { .id = "OVER" }, { .handler = x_over },
/*  72 */ { .handler = i_addname }, { .id = "R>" }, { .handler = x_r_from },
/*  75 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/*  77 */ { .handler = i_addname }, { .id = "R@" }, { .handler = x_r_fetch },
/*  80 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/*  82 */ { .handler = i_addname }, { .id = "ROT" }, { .handler = x_rot },
/*  85 */ { .handler = i_addname }, { .id = "SWAP" }, { .handler = x_swap },
/*  88 */ { .handler = i_addname }, { .id = "2>R" }, { .handler = x_two_to_r },
/*  91 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/*  93 */ { .handler = i_addname }, { .id = "2R>" }, { .handler = x_two_r_from },
/*  96 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/*  98 */ { .handler = i_addname }, { .id = "2R@" }, { .handler = x_two_r_fetch },
/* 101 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 103 */ { .handler = i_addname }, { .id = "PICK" }, { .handler = x_pick },
/* 106 */ { .handler = i_addname }, { .id = "+" }, { .handler = x_plus },
/* 109 */ { .handler = i_addname }, { .id = "-" }, { .handler = x_minus },
/* 112 */ { .handler = i_addname }, { .id = "2*" }, { .handler = x_two_star },
/* 115 */ { .handler = i_addname }, { .id = "2/" }, { .handler = x_two_slash },
/* 118 */ { .handler = i_addname }, { .id = "<" }, { .handler = x_less_than },
/* 121 */ { .handler = i_addname }, { .id = "=" }, { .handler = x_equals },
/* 124 */ { .handler = i_addname }, { .id = ">" }, { .handler = x_greater_than },
/* 127 */ { .handler = i_addname }, { .id = "AND" }, { .handler = x_and },
/* 130 */ { .handler = i_addname }, { .id = "INVERT" }, { .handler = x_invert },
/* 133 */ { .handler = i_addname }, { .id = "LSHIFT" }, { .handler = x_lshift },
/* 136 */ { .handler = i_addname }, { .id = "NEGATE" }, { .handler = x_negate },
/* 139 */ { .handler = i_addname }, { .id = "OR" }, { .handler = x_or },
/* 142 */ { .handler = i_addname }, { .id = "RSHIFT" }, { .handler = x_rshift },
/* 145 */ { .handler = i_addname }, { .id = "U<" }, { .handler = x_u_less },
/* 148 */ { .handler = i_addname }, { .id = "XOR" }, { .handler = x_xor },
/* 151 */ { .handler = i_addname }, { .id = "!" }, { .handler = x_store },
/* 154 */ { .handler = i_addname }, { .id = "@" }, { .handler = x_fetch },
/* 157 */ { .handler = i_addname }, { .id = "C!" }, { .handler = x_c_store },
/* 160 */ { .handler = i_addname }, { .id = "C@" }, { .handler = x_c_fetch },
/* 163 */ { .handler = i_addname }, { .id = "FILL" }, { .handler = x_fill },
/* 166 */ { .handler = i_addname }, { .id = "MOVE" }, { .handler = x_move },
/* 169 */ { .handler = i_addname }, { .id = "*" }, { .handler = x_star },
/* 172 */ { .handler = i_addname }, { .id = "/" }, { .handler = x_slash },
/* 175 */ { .handler = i_addname }, { .id = "/MOD" }, { .handler = x_slash_mod },
/* 178 */ { .handler = i_addname }, { .id = "M*" }, { .handler = x_m_star },
/* 181 */ { .handler = i_addname }, { .id = "MOD" }, { .handler = x_mod },
/* 184 */ { .handler = i_addname }, { .id = "UM*" }, { .handler = x_u_m_star },
/* 187 */ { .handler = i_addname }, { .id = "UM/MOD" }, { .handler = x_u_m_slash_mod },
/* 190 */ { .handler = i_addname }, { .id = "EMIT" }, { .handler = x_emit },
/* 193 */ { .handler = i_addname }, { .id = "'" }, { .handler = x_tick },
/* 196 */ { .handler = i_addname }, { .id = "EXIT" }, { .handler = x_exit },
/* 199 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 201 */ { .handler = i_addname }, { .id = "FIND" }, { .handler = x_find },
/* 204 */ { .handler = i_addname }, { .id = "SEARCH-WORDLIST" }, { .handler = x_search_wordlist },
/* 207 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.to_in) },
/* 209 */ { .handler = i_addname }, { .id = ">IN" }, { .handler = do_constant },
/* 212 */ { .handler = x_comma },
/* 213 */ { .handler = i_addname }, { .id = "ABORT" }, { .handler = x_abort },
/* 216 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.base) },
/* 218 */ { .handler = i_addname }, { .id = "BASE" }, { .handler = do_constant },
/* 221 */ { .handler = x_comma },
/* 222 */ { .handler = i_addname }, { .id = "CHAR" }, { .handler = x_char },
/* 225 */ { .handler = i_addname }, { .id = "DECIMAL" }, { .handler = x_decimal },
/* 228 */ { .handler = i_addname }, { .id = "EVALUATE" }, { .handler = x_evaluate },
/* 231 */ { .handler = i_addname }, { .id = "EXECUTE" }, { .handler = x_execute },
/* 234 */ { .handler = i_addname }, { .id = "LITERAL" }, { .handler = x_literal },
/* 237 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 239 */ { .handler = i_addname }, { .id = "POSTPONE" }, { .handler = x_postpone },
/* 242 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 244 */ { .handler = do_literal }, { .cell = (cell_ft) (do_postpone) },
/* 246 */ { .handler = meta_interpret },
/* 247 */     { .id = "," },
/* 248 */     { .id = NULL },
/* 249 */ { .handler = i_addname }, { .id = "QUIT" }, { .handler = x_quit },
/* 252 */ { .handler = i_addname }, { .id = "SOURCE" }, { .handler = x_source },
/* 255 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.state) },
/* 257 */ { .handler = i_addname }, { .id = "STATE" }, { .handler = do_constant },
/* 260 */ { .handler = x_comma },
/* 261 */ { .handler = i_addname }, { .id = "S\"" }, { .handler = x_s_quote },
/* 264 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 266 */ { .handler = do_literal }, { .cell = (cell_ft) (do_s_quote) },
/* 268 */ { .handler = meta_interpret },
/* 269 */     { .id = "," },
/* 270 */     { .id = NULL },
/* 271 */ { .handler = i_addname }, { .id = "[" }, { .handler = x_left_bracket },
/* 274 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 276 */ { .handler = i_addname }, { .id = "]" }, { .handler = x_right_bracket },
/* 279 */ { .handler = i_addname }, { .id = "C\"" }, { .handler = x_c_quote },
/* 282 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 284 */ { .handler = do_literal }, { .cell = (cell_ft) (do_c_quote) },
/* 286 */ { .handler = meta_interpret },
/* 287 */     { .id = "," },
/* 288 */     { .id = NULL },
/* 289 */ { .handler = i_addname }, { .id = "HEX" }, { .handler = x_hex },
/* 292 */ { .handler = i_addname }, { .id = "PARSE" }, { .handler = x_parse },
/* 295 */ { .handler = i_addname }, { .id = "REFILL" }, { .handler = x_refill },
/* 298 */ { .handler = i_addname }, { .id = "THROW" }, { .handler = x_throw },
/* 301 */ { .handler = i_addname }, { .id = "BIN" }, { .handler = x_bin },
/* 304 */ { .handler = i_addname }, { .id = "CLOSE-FILE" }, { .handler = x_close_file },
/* 307 */ { .handler = i_addname }, { .id = "CREATE-FILE" }, { .handler = x_create_file },
/* 310 */ { .handler = i_addname }, { .id = "FILE-POSITION" }, { .handler = x_file_position },
/* 313 */ { .handler = i_addname }, { .id = "FILE-SIZE" }, { .handler = x_file_size },
/* 316 */ { .handler = i_addname }, { .id = "OPEN-FILE" }, { .handler = x_open_file },
/* 319 */ { .handler = i_addname }, { .id = "R/O" }, { .handler = x_r_o },
/* 322 */ { .handler = i_addname }, { .id = "R/W" }, { .handler = x_r_w },
/* 325 */ { .handler = i_addname }, { .id = "READ-FILE" }, { .handler = x_read_file },
/* 328 */ { .handler = i_addname }, { .id = "REPOSITION-FILE" }, { .handler = x_reposition_file },
/* 331 */ { .handler = i_addname }, { .id = "W/O" }, { .handler = x_w_o },
/* 334 */ { .handler = i_addname }, { .id = "WRITE-FILE" }, { .handler = x_write_file },
/* 337 */ { .handler = i_startname }, { .id = "CHARS" }, { .handler = do_colon },
/* 340 */ { .handler = meta_compile },
/* 341 */     { .id = "EXIT" },
/* 342 */     { .id = NULL },
/* 343 */ { .handler = x_linkname },
/* 344 */ { .handler = i_startname }, { .id = "CHAR+" }, { .handler = do_colon },
/* 347 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/* 349 */ { .handler = meta_interpret },
/* 350 */     { .id = "LITERAL" },
/* 351 */     { .id = NULL },
/* 352 */ { .handler = meta_compile },
/* 353 */     { .id = "+" },
/* 354 */     { .id = "EXIT" },
/* 355 */     { .id = NULL },
/* 356 */ { .handler = x_linkname },
/* 357 */ { .handler = i_startname }, { .id = "CELLS" }, { .handler = do_colon },
/* 360 */ { .handler = do_literal }, { .cell = (cell_ft) (CELL_SIZE) },
/* 362 */ { .handler = meta_interpret },
/* 363 */     { .id = "LITERAL" },
/* 364 */     { .id = NULL },
/* 365 */ { .handler = meta_compile },
/* 366 */     { .id = "*" },
/* 367 */     { .id = "EXIT" },
/* 368 */     { .id = NULL },
/* 369 */ { .handler = x_linkname },
/* 370 */ { .handler = i_startname }, { .id = "CELL+" }, { .handler = do_colon },
/* 373 */ { .handler = do_literal }, { .cell = (cell_ft) (CELL_SIZE) },
/* 375 */ { .handler = meta_interpret },
/* 376 */     { .id = "LITERAL" },
/* 377 */     { .id = NULL },
/* 378 */ { .handler = meta_compile },
/* 379 */     { .id = "+" },
/* 380 */     { .id = "EXIT" },
/* 381 */     { .id = NULL },
/* 382 */ { .handler = x_linkname },
/* 383 */ { .handler = i_addname }, { .id = "BRANCH" }, { .handler = do_skip },
/* 386 */ { .handler = i_addname }, { .id = "?BRANCH" }, { .handler = do_fskip },
/* 389 */ { .handler = i_startname }, { .id = "<MARK" }, { .handler = do_colon },
/* 392 */ { .handler = meta_compile },
/* 393 */     { .id = "here" },
/* 394 */     { .id = "EXIT" },
/* 395 */     { .id = NULL },
/* 396 */ { .handler = x_linkname },
/* 397 */ { .handler = i_startname }, { .id = "<RESOLVE" }, { .handler = do_colon },
/* 400 */ { .handler = meta_compile },
/* 401 */     { .id = "here" },
/* 402 */     { .id = "-" },
/* 403 */     { .id = NULL },
/* 404 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/* 406 */ { .handler = meta_interpret },
/* 407 */     { .id = "cells" },
/* 408 */     { .id = "LITERAL" },
/* 409 */     { .id = NULL },
/* 410 */ { .handler = meta_compile },
/* 411 */     { .id = "/" },
/* 412 */     { .id = "," },
/* 413 */     { .id = "EXIT" },
/* 414 */     { .id = NULL },
/* 415 */ { .handler = x_linkname },
/* 416 */ { .handler = i_startname }, { .id = ">MARK" }, { .handler = do_colon },
/* 419 */ { .handler = meta_compile },
/* 420 */     { .id = "here" },
/* 421 */     { .id = NULL },
/* 422 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/* 424 */ { .handler = meta_interpret },
/* 425 */     { .id = "cells" },
/* 426 */     { .id = "LITERAL" },
/* 427 */     { .id = NULL },
/* 428 */ { .handler = meta_compile },
/* 429 */     { .id = "allot" },
/* 430 */     { .id = "EXIT" },
/* 431 */     { .id = NULL },
/* 432 */ { .handler = x_linkname },
/* 433 */ { .handler = i_startname }, { .id = ">RESOLVE" }, { .handler = do_colon },
/* 436 */ { .handler = meta_compile },
/* 437 */     { .id = "here" },
/* 438 */     { .id = "over" },
/* 439 */     { .id = "-" },
/* 440 */     { .id = NULL },
/* 441 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/* 443 */ { .handler = meta_interpret },
/* 444 */     { .id = "cells" },
/* 445 */     { .id = "LITERAL" },
/* 446 */     { .id = NULL },
/* 447 */ { .handler = meta_compile },
/* 448 */     { .id = "/" },
/* 449 */     { .id = "swap" },
/* 450 */     { .id = "!" },
/* 451 */     { .id = "EXIT" },
/* 452 */     { .id = NULL },
/* 453 */ { .handler = x_linkname },
/* 454 */ { .handler = i_startname }, { .id = ">BRANCH" }, { .handler = do_colon },
/* 457 */ { .handler = i_lookup }, { .id = "branch" },
/* 459 */ { .handler = meta_interpret },
/* 460 */     { .id = "LITERAL" },
/* 461 */     { .id = NULL },
/* 462 */ { .handler = meta_compile },
/* 463 */     { .id = "," },
/* 464 */     { .id = ">mark" },
/* 465 */     { .id = "EXIT" },
/* 466 */     { .id = NULL },
/* 467 */ { .handler = x_linkname },
/* 468 */ { .handler = i_startname }, { .id = "<BRANCH" }, { .handler = do_colon },
/* 471 */ { .handler = i_lookup }, { .id = "branch" },
/* 473 */ { .handler = meta_interpret },
/* 474 */     { .id = "LITERAL" },
/* 475 */     { .id = NULL },
/* 476 */ { .handler = meta_compile },
/* 477 */     { .id = "," },
/* 478 */     { .id = "<resolve" },
/* 479 */     { .id = "EXIT" },
/* 480 */     { .id = NULL },
/* 481 */ { .handler = x_linkname },
/* 482 */ { .handler = i_startname }, { .id = ">?BRANCH" }, { .handler = do_colon },
/* 485 */ { .handler = i_lookup }, { .id = "?branch" },
/* 487 */ { .handler = meta_interpret },
/* 488 */     { .id = "LITERAL" },
/* 489 */     { .id = NULL },
/* 490 */ { .handler = meta_compile },
/* 491 */     { .id = "," },
/* 492 */     { .id = ">mark" },
/* 493 */     { .id = "EXIT" },
/* 494 */     { .id = NULL },
/* 495 */ { .handler = x_linkname },
/* 496 */ { .handler = i_startname }, { .id = "<?BRANCH" }, { .handler = do_colon },
/* 499 */ { .handler = i_lookup }, { .id = "?branch" },
/* 501 */ { .handler = meta_interpret },
/* 502 */     { .id = "LITERAL" },
/* 503 */     { .id = NULL },
/* 504 */ { .handler = meta_compile },
/* 505 */     { .id = "," },
/* 506 */     { .id = "<resolve" },
/* 507 */     { .id = "EXIT" },
/* 508 */     { .id = NULL },
/* 509 */ { .handler = x_linkname },
/* 510 */ { .handler = i_startname }, { .id = "BEGIN" }, { .handler = do_colon },
/* 513 */ { .handler = meta_compile },
/* 514 */     { .id = "<mark" },
/* 515 */     { .id = "EXIT" },
/* 516 */     { .id = NULL },
/* 517 */ { .handler = x_linkname },
/* 518 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 520 */ { .handler = i_startname }, { .id = "THEN" }, { .handler = do_colon },
/* 523 */ { .handler = meta_compile },
/* 524 */     { .id = ">resolve" },
/* 525 */     { .id = "EXIT" },
/* 526 */     { .id = NULL },
/* 527 */ { .handler = x_linkname },
/* 528 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 530 */ { .handler = i_startname }, { .id = "IF" }, { .handler = do_colon },
/* 533 */ { .handler = meta_compile },
/* 534 */     { .id = ">?branch" },
/* 535 */     { .id = "EXIT" },
/* 536 */     { .id = NULL },
/* 537 */ { .handler = x_linkname },
/* 538 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 540 */ { .handler = i_startname }, { .id = "UNTIL" }, { .handler = do_colon },
/* 543 */ { .handler = meta_compile },
/* 544 */     { .id = "<?branch" },
/* 545 */     { .id = "EXIT" },
/* 546 */     { .id = NULL },
/* 547 */ { .handler = x_linkname },
/* 548 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 550 */ { .handler = i_startname }, { .id = "REPEAT" }, { .handler = do_colon },
/* 553 */ { .handler = meta_compile },
/* 554 */     { .id = "<branch" },
/* 555 */     { .id = "THEN" },
/* 556 */     { .id = "EXIT" },
/* 557 */     { .id = NULL },
/* 558 */ { .handler = x_linkname },
/* 559 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 561 */ { .handler = i_startname }, { .id = "ELSE" }, { .handler = do_colon },
/* 564 */ { .handler = meta_compile },
/* 565 */     { .id = ">branch" },
/* 566 */     { .id = "swap" },
/* 567 */     { .id = "THEN" },
/* 568 */     { .id = "EXIT" },
/* 569 */     { .id = NULL },
/* 570 */ { .handler = x_linkname },
/* 571 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 573 */ { .handler = i_startname }, { .id = "WHILE" }, { .handler = do_colon },
/* 576 */ { .handler = meta_compile },
/* 577 */     { .id = "IF" },
/* 578 */     { .id = "swap" },
/* 579 */     { .id = "EXIT" },
/* 580 */     { .id = NULL },
/* 581 */ { .handler = x_linkname },
/* 582 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 584 */ { .handler = i_addname }, { .id = "LEAVERS" }, { .handler = do_variable },
/* 587 */ { .handler = do_literal }, { .cell = (cell_ft) (CELL_SIZE) },
/* 589 */ { .handler = x_allot },
/* 590 */ { .handler = do_literal }, { .cell = (cell_ft) (0x0) },
/* 592 */ { .handler = meta_interpret },
/* 593 */     { .id = "leavers" },
/* 594 */     { .id = "!" },
/* 595 */     { .id = "here" },
/* 596 */     { .id = NULL },
/* 597 */ { .handler = do_literal }, { .cell = (cell_ft) (do_do) },
/* 599 */ { .handler = meta_interpret },
/* 600 */     { .id = "," },
/* 601 */     { .id = NULL },
/* 602 */ { .handler = i_startname }, { .id = "DO" }, { .handler = do_colon },
/* 605 */ { .handler = meta_interpret },
/* 606 */     { .id = "swap" },
/* 607 */     { .id = "LITERAL" },
/* 608 */     { .id = NULL },
/* 609 */ { .handler = meta_compile },
/* 610 */     { .id = "," },
/* 611 */     { .id = "leavers" },
/* 612 */     { .id = "@" },
/* 613 */     { .id = NULL },
/* 614 */ { .handler = do_literal }, { .cell = (cell_ft) (0x0) },
/* 616 */ { .handler = meta_interpret },
/* 617 */     { .id = "LITERAL" },
/* 618 */     { .id = NULL },
/* 619 */ { .handler = meta_compile },
/* 620 */     { .id = "leavers" },
/* 621 */     { .id = "!" },
/* 622 */     { .id = "BEGIN" },
/* 623 */     { .id = "EXIT" },
/* 624 */     { .id = NULL },
/* 625 */ { .handler = x_linkname },
/* 626 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 628 */ { .handler = i_startname }, { .id = "LEAVE" }, { .handler = do_colon },
/* 631 */ { .handler = meta_compile },
/* 632 */     { .id = ">branch" },
/* 633 */     { .id = "leavers" },
/* 634 */     { .id = "@" },
/* 635 */     { .id = "over" },
/* 636 */     { .id = "!" },
/* 637 */     { .id = "leavers" },
/* 638 */     { .id = "!" },
/* 639 */     { .id = "EXIT" },
/* 640 */     { .id = NULL },
/* 641 */ { .handler = x_linkname },
/* 642 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 644 */ { .handler = i_addname }, { .id = "UNLOOP" }, { .handler = x_unloop },
/* 647 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 649 */ { .handler = meta_interpret },
/* 650 */     { .id = "here" },
/* 651 */     { .id = NULL },
/* 652 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/* 654 */ { .handler = meta_interpret },
/* 655 */     { .id = "cells" },
/* 656 */     { .id = "-" },
/* 657 */     { .id = NULL },
/* 658 */ { .handler = i_addname }, { .id = "I" }, { .handler = x_i },
/* 661 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 663 */ { .handler = i_addname }, { .id = "J" }, { .handler = x_j },
/* 666 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 668 */ { .handler = meta_interpret },
/* 669 */     { .id = "here" },
/* 670 */     { .id = NULL },
/* 671 */ { .handler = do_literal }, { .cell = (cell_ft) (do_plus_loop) },
/* 673 */ { .handler = meta_interpret },
/* 674 */     { .id = "," },
/* 675 */     { .id = NULL },
/* 676 */ { .handler = i_startname }, { .id = "+LOOP" }, { .handler = do_colon },
/* 679 */ { .handler = meta_interpret },
/* 680 */     { .id = "swap" },
/* 681 */     { .id = "LITERAL" },
/* 682 */     { .id = NULL },
/* 683 */ { .handler = meta_compile },
/* 684 */     { .id = "," },
/* 685 */     { .id = "UNTIL" },
/* 686 */     { .id = "leavers" },
/* 687 */     { .id = "@" },
/* 688 */     { .id = "swap" },
/* 689 */     { .id = "leavers" },
/* 690 */     { .id = "!" },
/* 691 */     { .id = NULL },
/* 692 */ { .handler = meta_interpret },
/* 693 */     { .id = "BEGIN" },
/* 694 */     { .id = NULL },
/* 695 */ { .handler = meta_compile },
/* 696 */     { .id = "dup" },
/* 697 */     { .id = NULL },
/* 698 */ { .handler = meta_interpret },
/* 699 */     { .id = "WHILE" },
/* 700 */     { .id = NULL },
/* 701 */ { .handler = meta_compile },
/* 702 */     { .id = "dup" },
/* 703 */     { .id = "THEN" },
/* 704 */     { .id = "@" },
/* 705 */     { .id = NULL },
/* 706 */ { .handler = meta_interpret },
/* 707 */     { .id = "REPEAT" },
/* 708 */     { .id = NULL },
/* 709 */ { .handler = meta_compile },
/* 710 */     { .id = "drop" },
/* 711 */     { .id = NULL },
/* 712 */ { .handler = meta_interpret },
/* 713 */     { .id = "swap" },
/* 714 */     { .id = "LITERAL" },
/* 715 */     { .id = NULL },
/* 716 */ { .handler = meta_compile },
/* 717 */     { .id = "," },
/* 718 */     { .id = "EXIT" },
/* 719 */     { .id = NULL },
/* 720 */ { .handler = x_linkname },
/* 721 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 723 */ { .handler = i_startname }, { .id = "+!" }, { .handler = do_colon },
/* 726 */ { .handler = meta_compile },
/* 727 */     { .id = "dup" },
/* 728 */     { .id = ">r" },
/* 729 */     { .id = "@" },
/* 730 */     { .id = "+" },
/* 731 */     { .id = "r>" },
/* 732 */     { .id = "!" },
/* 733 */     { .id = "EXIT" },
/* 734 */     { .id = NULL },
/* 735 */ { .handler = x_linkname },
/* 736 */ { .handler = i_startname }, { .id = "2!" }, { .handler = do_colon },
/* 739 */ { .handler = meta_compile },
/* 740 */     { .id = "swap" },
/* 741 */     { .id = "over" },
/* 742 */     { .id = "!" },
/* 743 */     { .id = "cell+" },
/* 744 */     { .id = "!" },
/* 745 */     { .id = "EXIT" },
/* 746 */     { .id = NULL },
/* 747 */ { .handler = x_linkname },
/* 748 */ { .handler = i_startname }, { .id = "2@" }, { .handler = do_colon },
/* 751 */ { .handler = meta_compile },
/* 752 */     { .id = "dup" },
/* 753 */     { .id = "cell+" },
/* 754 */     { .id = "@" },
/* 755 */     { .id = "swap" },
/* 756 */     { .id = "@" },
/* 757 */     { .id = "EXIT" },
/* 758 */     { .id = NULL },
/* 759 */ { .handler = x_linkname },
/* 760 */ { .handler = i_startname }, { .id = "ALIGNED" }, { .handler = do_colon },
/* 763 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/* 765 */ { .handler = meta_interpret },
/* 766 */     { .id = "cells" },
/* 767 */     { .id = NULL },
/* 768 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1) },
/* 770 */ { .handler = meta_interpret },
/* 771 */     { .id = "-" },
/* 772 */     { .id = "LITERAL" },
/* 773 */     { .id = NULL },
/* 774 */ { .handler = meta_compile },
/* 775 */     { .id = "+" },
/* 776 */     { .id = NULL },
/* 777 */ { .handler = do_literal }, { .cell = (cell_ft) (-0x1) },
/* 779 */ { .handler = meta_interpret },
/* 780 */     { .id = "cells" },
/* 781 */     { .id = "LITERAL" },
/* 782 */     { .id = NULL },
/* 783 */ { .handler = meta_compile },
/* 784 */     { .id = "and" },
/* 785 */     { .id = "EXIT" },
/* 786 */     { .id = NULL },
/* 787 */ { .handler = x_linkname },
/* 788 */ { .handler = i_startname }, { .id = "COUNT" }, { .handler = do_colon },
/* 791 */ { .handler = meta_compile },
/* 792 */     { .id = "dup" },
/* 793 */     { .id = "char+" },
/* 794 */     { .id = "swap" },
/* 795 */     { .id = "c@" },
/* 796 */     { .id = "EXIT" },
/* 797 */     { .id = NULL },
/* 798 */ { .handler = x_linkname },
/* 799 */ { .handler = i_startname }, { .id = "ERASE" }, { .handler = do_colon },
/* 802 */ { .handler = meta_compile },
/* 803 */     { .id = "CHARS" },
/* 804 */     { .id = NULL },
/* 805 */ { .handler = do_literal }, { .cell = (cell_ft) (0x0) },
/* 807 */ { .handler = meta_interpret },
/* 808 */     { .id = "LITERAL" },
/* 809 */     { .id = NULL },
/* 810 */ { .handler = meta_compile },
/* 811 */     { .id = "FILL" },
/* 812 */     { .id = "EXIT" },
/* 813 */     { .id = NULL },
/* 814 */ { .handler = x_linkname },
/* 815 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.forth_wordlist) },
/* 817 */ { .handler = i_addname }, { .id = "FORTH-WORDLIST" }, { .handler = do_constant },
/* 820 */ { .handler = x_comma },
/* 821 */ { .handler = i_startname }, { .id = "GET-CURRENT" }, { .handler = do_colon },
/* 824 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.current) },
/* 826 */ { .handler = meta_interpret },
/* 827 */     { .id = "LITERAL" },
/* 828 */     { .id = NULL },
/* 829 */ { .handler = meta_compile },
/* 830 */     { .id = "@" },
/* 831 */     { .id = "EXIT" },
/* 832 */     { .id = NULL },
/* 833 */ { .handler = x_linkname },
/* 834 */ { .handler = i_startname }, { .id = "SET-CURRENT" }, { .handler = do_colon },
/* 837 */ { .handler = do_literal }, { .cell = (cell_ft) (&DICT.current) },
/* 839 */ { .handler = meta_interpret },
/* 840 */     { .id = "LITERAL" },
/* 841 */     { .id = NULL },
/* 842 */ { .handler = meta_compile },
/* 843 */     { .id = "!" },
/* 844 */     { .id = "EXIT" },
/* 845 */     { .id = NULL },
/* 846 */ { .handler = x_linkname },
/* 847 */ { .handler = i_addname }, { .id = "startname" }, { .handler = x_startname },
/* 850 */ { .handler = i_addname }, { .id = "linkname" }, { .handler = x_linkname },
/* 853 */ { .handler = i_startname }, { .id = "addname" }, { .handler = do_colon },
/* 856 */ { .handler = meta_compile },
/* 857 */     { .id = "startname" },
/* 858 */     { .id = "linkname" },
/* 859 */     { .id = "EXIT" },
/* 860 */     { .id = NULL },
/* 861 */ { .handler = x_linkname },
/* 862 */ { .handler = i_startname }, { .id = ":" }, { .handler = do_colon },
/* 865 */ { .handler = do_literal }, { .cell = (cell_ft) (do_colon) },
/* 867 */ { .handler = meta_interpret },
/* 868 */     { .id = "LITERAL" },
/* 869 */     { .id = NULL },
/* 870 */ { .handler = meta_compile },
/* 871 */     { .id = "startname" },
/* 872 */     { .id = "]" },
/* 873 */     { .id = "EXIT" },
/* 874 */     { .id = NULL },
/* 875 */ { .handler = x_linkname },
/* 876 */ { .handler = i_startname }, { .id = ";" }, { .handler = do_colon },
/* 879 */ { .handler = i_lookup }, { .id = "EXIT" },
/* 881 */ { .handler = meta_interpret },
/* 882 */     { .id = "LITERAL" },
/* 883 */     { .id = NULL },
/* 884 */ { .handler = meta_compile },
/* 885 */     { .id = "," },
/* 886 */     { .id = "linkname" },
/* 887 */     { .id = "[" },
/* 888 */     { .id = "EXIT" },
/* 889 */     { .id = NULL },
/* 890 */ { .handler = x_linkname },
/* 891 */ { .handler = i_setflags }, { .cell = (cell_ft) (0xC0) },
/* 893 */ { .handler = i_startname }, { .id = "created?" }, { .handler = do_colon },
/* 896 */ { .handler = meta_compile },
/* 897 */     { .id = "dup" },
/* 898 */     { .id = "@" },
/* 899 */     { .id = NULL },
/* 900 */ { .handler = do_literal }, { .cell = (cell_ft) (do_create) },
/* 902 */ { .handler = meta_interpret },
/* 903 */     { .id = "LITERAL" },
/* 904 */     { .id = NULL },
/* 905 */ { .handler = meta_compile },
/* 906 */     { .id = "=" },
/* 907 */     { .id = "invert" },
/* 908 */     { .id = NULL },
/* 909 */ { .handler = meta_interpret },
/* 910 */     { .id = "IF" },
/* 911 */     { .id = NULL },
/* 912 */ { .handler = do_literal }, { .cell = (cell_ft) (-0x1F) },
/* 914 */ { .handler = meta_interpret },
/* 915 */     { .id = "LITERAL" },
/* 916 */     { .id = NULL },
/* 917 */ { .handler = meta_compile },
/* 918 */     { .id = "throw" },
/* 919 */     { .id = NULL },
/* 920 */ { .handler = meta_interpret },
/* 921 */     { .id = "THEN" },
/* 922 */     { .id = NULL },
/* 923 */ { .handler = meta_compile },
/* 924 */     { .id = "EXIT" },
/* 925 */     { .id = NULL },
/* 926 */ { .handler = x_linkname },
/* 927 */ { .handler = i_startname }, { .id = ">BODY" }, { .handler = do_colon },
/* 930 */ { .handler = meta_compile },
/* 931 */     { .id = "created?" },
/* 932 */     { .id = NULL },
/* 933 */ { .handler = do_literal }, { .cell = (cell_ft) (0x2) },
/* 935 */ { .handler = meta_interpret },
/* 936 */     { .id = "cells" },
/* 937 */     { .id = "LITERAL" },
/* 938 */     { .id = NULL },
/* 939 */ { .handler = meta_compile },
/* 940 */     { .id = "+" },
/* 941 */     { .id = "EXIT" },
/* 942 */     { .id = NULL },
/* 943 */ { .handler = x_linkname },
/* 944 */ { .handler = i_startname }, { .id = "CONSTANT" }, { .handler = do_colon },
/* 947 */ { .handler = do_literal }, { .cell = (cell_ft) (do_constant) },
/* 949 */ { .handler = meta_interpret },
/* 950 */     { .id = "LITERAL" },
/* 951 */     { .id = NULL },
/* 952 */ { .handler = meta_compile },
/* 953 */     { .id = "addname" },
/* 954 */     { .id = "," },
/* 955 */     { .id = "EXIT" },
/* 956 */     { .id = NULL },
/* 957 */ { .handler = x_linkname },
/* 958 */ { .handler = i_startname }, { .id = "current-name" }, { .handler = do_colon },
/* 961 */ { .handler = meta_compile },
/* 962 */     { .id = "get-current" },
/* 963 */     { .id = "@" },
/* 964 */     { .id = "EXIT" },
/* 965 */     { .id = NULL },
/* 966 */ { .handler = x_linkname },
/* 967 */ { .handler = i_startname }, { .id = "name>xt" }, { .handler = do_colon },
/* 970 */ { .handler = meta_compile },
/* 971 */     { .id = "cell+" },
/* 972 */     { .id = "count" },
/* 973 */     { .id = NULL },
/* 974 */ { .handler = do_literal }, { .cell = (cell_ft) (0x1F) },
/* 976 */ { .handler = meta_interpret },
/* 977 */     { .id = "LITERAL" },
/* 978 */     { .id = NULL },
/* 979 */ { .handler = meta_compile },
/* 980 */     { .id = "and" },
/* 981 */     { .id = "chars" },
/* 982 */     { .id = "+" },
/* 983 */     { .id = "aligned" },
/* 984 */     { .id = "EXIT" },
/* 985 */     { .id = NULL },
/* 986 */ { .handler = x_linkname },
/* 987 */ { .handler = i_startname }, { .id = "DOES>" }, { .handler = do_colon },
/* 990 */ { .handler = meta_compile },
/* 991 */     { .id = "r>" },
/* 992 */     { .id = "current-name" },
/* 993 */     { .id = "name>xt" },
/* 994 */     { .id = "created?" },
/* 995 */     { .id = "cell+" },
/* 996 */     { .id = "!" },
/* 997 */     { .id = "EXIT" },
/* 998 */     { .id = NULL },
/* 999 */ { .handler = x_linkname },
/* 1000 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x40) },
/* 1002 */ { .handler = i_startname }, { .id = "CREATE" }, { .handler = do_colon },
/* 1005 */ { .handler = do_literal }, { .cell = (cell_ft) (do_create) },
/* 1007 */ { .handler = meta_interpret },
/* 1008 */     { .id = "LITERAL" },
/* 1009 */     { .id = NULL },
/* 1010 */ { .handler = meta_compile },
/* 1011 */     { .id = "addname" },
/* 1012 */     { .id = NULL },
/* 1013 */ { .handler = do_literal }, { .cell = (cell_ft) (0x0) },
/* 1015 */ { .handler = meta_interpret },
/* 1016 */     { .id = "LITERAL" },
/* 1017 */     { .id = NULL },
/* 1018 */ { .handler = meta_compile },
/* 1019 */     { .id = "," },
/* 1020 */     { .id = "DOES>" },
/* 1021 */     { .id = "EXIT" },
/* 1022 */     { .id = NULL },
/* 1023 */ { .handler = x_linkname },
/* 1024 */ { .handler = i_startname }, { .id = "flags!" }, { .handler = do_colon },
/* 1027 */ { .handler = meta_compile },
/* 1028 */     { .id = "current-name" },
/* 1029 */     { .id = "cell+" },
/* 1030 */     { .id = "dup" },
/* 1031 */     { .id = ">r" },
/* 1032 */     { .id = "c@" },
/* 1033 */     { .id = "or" },
/* 1034 */     { .id = "r>" },
/* 1035 */     { .id = "c!" },
/* 1036 */     { .id = "EXIT" },
/* 1037 */     { .id = NULL },
/* 1038 */ { .handler = x_linkname },
/* 1039 */ { .handler = i_startname }, { .id = "IMMEDIATE" }, { .handler = do_colon },
/* 412 */ { .handler = do_literal }, { .cell = (cell_ft) (0x80) },
/* 414 */ { .handler = meta_interpret },
/* 415 */     { .id = "LITERAL" },
/* 1046 */     { .id = NULL },
/* 1047 */ { .handler = meta_compile },
/* 1048 */     { .id = "flags!" },
/* 1049 */     { .id = "EXIT" },
/* 1050 */     { .id = NULL },
/* 1051 */ { .handler = x_linkname },
/* 1052 */ { .handler = i_startname }, { .id = "NO-INTERPRET" }, { .handler = do_colon },
/* 41F */ { .handler = do_literal }, { .cell = (cell_ft) (0x40) },
/* 421 */ { .handler = meta_interpret },
/* 422 */     { .id = "LITERAL" },
/* 1059 */     { .id = NULL },
/* 1060 */ { .handler = meta_compile },
/* 1061 */     { .id = "flags!" },
/* 1062 */     { .id = "EXIT" },
/* 1063 */     { .id = NULL },
/* 1064 */ { .handler = x_linkname },
/* 1065 */ { .handler = i_startname }, { .id = "COMPILE-ONLY" }, { .handler = do_colon },
/* 42C */ { .handler = do_literal }, { .cell = (cell_ft) (0xC0) },
/* 42E */ { .handler = meta_interpret },
/* 42F */     { .id = "LITERAL" },
/* 1072 */     { .id = NULL },
/* 1073 */ { .handler = meta_compile },
/* 1074 */     { .id = "flags!" },
/* 1075 */     { .id = "EXIT" },
/* 1076 */     { .id = NULL },
/* 1077 */ { .handler = x_linkname },
/* 1078 */ { .handler = i_startname }, { .id = "VARIABLE" }, { .handler = do_colon },
/* 1081 */ { .handler = do_literal }, { .cell = (cell_ft) (do_variable) },
/* 1083 */ { .handler = meta_interpret },
/* 1084 */     { .id = "LITERAL" },
/* 1085 */     { .id = NULL },
/* 1086 */ { .handler = meta_compile },
/* 1087 */     { .id = "addname" },
/* 1088 */     { .id = NULL },
/* 1089 */ { .handler = do_literal }, { .cell = (cell_ft) (0x0) },
/* 1091 */ { .handler = meta_interpret },
/* 1092 */     { .id = "LITERAL" },
/* 1093 */     { .id = NULL },
/* 1094 */ { .handler = meta_compile },
/* 1095 */     { .id = "," },
/* 1096 */     { .id = "EXIT" },
/* 1097 */     { .id = NULL },
/* 1098 */ { .handler = x_linkname },
/* 1099 */ { .handler = i_startname }, { .id = "(" }, { .handler = do_colon },
/* 1102 */ { .handler = do_literal }, { .cell = (cell_ft) (0x29) },
/* 1104 */ { .handler = meta_interpret },
/* 1105 */     { .id = "LITERAL" },
/* 1106 */     { .id = NULL },
/* 1107 */ { .handler = meta_compile },
/* 1108 */     { .id = "parse" },
/* 1109 */     { .id = "drop" },
/* 1110 */     { .id = "drop" },
/* 1111 */     { .id = "EXIT" },
/* 1112 */     { .id = NULL },
/* 1113 */ { .handler = x_linkname },
/* 1114 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x80) },
/* 1116 */ { .handler = i_startname }, { .id = "\\" }, { .handler = do_colon },
/* 1119 */ { .handler = meta_compile },
/* 1120 */     { .id = "source" },
/* 1121 */     { .id = ">in" },
/* 1122 */     { .id = "!" },
/* 1123 */     { .id = "drop" },
/* 1124 */     { .id = "EXIT" },
/* 1125 */     { .id = NULL },
/* 1126 */ { .handler = x_linkname },
/* 1127 */ { .handler = i_setflags }, { .cell = (cell_ft) (0x80) },
END_DIRECT // }
