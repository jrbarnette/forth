/*
 * Copyright 2024, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cforth.h"


#define N(val)		{ .value = (val) },
#define X(tok)		{ .xt = tok },
#define H(hdlr)		{ .handler = hdlr },
#define L(n)		X(DO_LITERAL) N(n)

#define DEF_PRIM(nm, hdlr)	code_body_ft nm[] = { H(hdlr) }
#define CODE(nm)		code_body_ft nm[] = { H(do_colon)
#define END_CODE		X(EXIT) }

DEF_PRIM(DO_LITERAL, do_literal);
DEF_PRIM(EXECUTE, x_execute);
DEF_PRIM(EXIT, x_exit);


struct test_case {
    char *		name;
    struct fargs	input;
    struct fargs	expect;
    xt_ft		exec_token;
};

struct test_suite {
    char *		name;
    struct test_case *	test_list;
};


/*
 * Tests for stuff in execute.m4
 */

CODE(NO_OP) END_CODE;
CODE(TEST_LIT) L(F_TRUE) END_CODE;

static struct test_case
execute_tests[] = {
    { "no-op", { 0 }, { 0 }, NO_OP },			// ( -- )
    { "lit", { 0 }, { 1, F_TRUE }, TEST_LIT },		// ( -- true )
    { NULL },
};

static struct test_suite
execute_suite = { "execute", execute_tests };


/*
 * Tests for stuff in arithops.m4
 */
DEF_PRIM(PLUS, x_plus);
DEF_PRIM(MINUS, x_minus);
DEF_PRIM(TWO_STAR, x_two_star);
DEF_PRIM(TWO_SLASH, x_two_slash);
DEF_PRIM(LESS_THAN, x_less_than);
DEF_PRIM(EQUALS, x_equals);
DEF_PRIM(GREATER_THAN, x_greater_than);
DEF_PRIM(AND, x_and);
DEF_PRIM(INVERT, x_invert);
DEF_PRIM(LSHIFT, x_lshift);
DEF_PRIM(NEGATE, x_negate);
DEF_PRIM(OR, x_or);
DEF_PRIM(RSHIFT, x_rshift);
DEF_PRIM(U_LESS, x_u_less);
DEF_PRIM(XOR, x_xor);

static struct test_case
arithops_tests[] = {
    { "+",    { 2, 1, 2 },     { 1, 3 },	PLUS },
    { "-",    { 2, 3, 2 },     { 1, 1 },	MINUS },
    { "2*",   { 1, 3 },        { 1, 6 },	TWO_STAR },
    { "2/",   { 1, -2 },       { 1, -1 },	TWO_SLASH },
    { "and",  { 2, 0xc, 0xa }, { 1, 0x8 },	AND },
    { "or",   { 2, 0xc, 0xa }, { 1, 0xe },	OR },
    { "xor",  { 2, 0xc, 0xa }, { 1, 0x6 },	XOR },

    { "invert", { 1, F_FALSE }, { 1, F_TRUE },	INVERT },
    { "negate", { 1, 1 },       { 1, -1 },	NEGATE },
    { "lshift", { 2, 3, 1 },    { 1, 6 },	LSHIFT },
    { "rshift", { 2, 3, 1 },    { 1, 1 },	RSHIFT },

    { "< true",  { 2, 1, 2 },   { 1, F_TRUE },	LESS_THAN },
    { "< false", { 2, 2, 1 },   { 1, F_FALSE },	LESS_THAN },
    { "= true",  { 2, 1, 1 },   { 1, F_TRUE },	EQUALS },
    { "= false", { 2, 1, 2 },   { 1, F_FALSE },	EQUALS },
    { "> true",  { 2, 2, 1 },   { 1, F_TRUE },	GREATER_THAN },
    { "> false", { 2, 1, 2 },   { 1, F_FALSE },	GREATER_THAN },
    { "u< true", { 2, 0, -1 },  { 1, F_TRUE },	U_LESS },
    { "u< false", { 2, -1, 0 }, { 1, F_FALSE },	U_LESS },

    { NULL },
};

static struct test_suite
arithops_suite = { "arithops", arithops_tests };


/*
 * Tests for stuff in stackops.m4
 */
DEF_PRIM(TO_R, x_to_r);
DEF_PRIM(QUESTION_DUP, x_question_dup);
DEF_PRIM(DROP, x_drop);
DEF_PRIM(DUP, x_dup);
DEF_PRIM(OVER, x_over);
DEF_PRIM(R_FROM, x_r_from);
DEF_PRIM(R_FETCH, x_r_fetch);
DEF_PRIM(ROT, x_rot);
DEF_PRIM(SWAP, x_swap);
DEF_PRIM(TWO_TO_R, x_two_to_r);
DEF_PRIM(TWO_R_FROM, x_two_r_from);
DEF_PRIM(TWO_R_FETCH, x_two_r_fetch);
DEF_PRIM(PICK, x_pick);
DEF_PRIM(ROLL, x_roll);

CODE(TEST_RSTACK) X(TO_R) X(INVERT) X(R_FROM) END_CODE;
CODE(TEST_RFETCH) X(TO_R) X(R_FETCH) X(R_FROM) END_CODE;
CODE(TEST_TWO_TO_R) X(TWO_TO_R) X(R_FROM) X(R_FROM) END_CODE;
CODE(TEST_TWO_R_FROM) X(TO_R) X(TO_R) X(TWO_R_FROM) END_CODE;
CODE(TEST_TWO_R_FETCH)
    X(TWO_TO_R) X(TWO_R_FETCH) X(TWO_R_FROM) X(DROP) X(DROP)
END_CODE;

static struct test_case
stackops_tests[] = {
    { "drop", { 1, 1 },       { 0 },			DROP },
    { "dup",  { 1, 1 },       { 2, 1, 1 },		DUP },
    { "over", { 2, 1, 2 },    { 3, 1, 2, 1 },		OVER },
    { "rot",  { 3, 1, 2, 3 }, { 3, 2, 3, 1 },		ROT },
    { "swap", { 2, 1, 2 },    { 2, 2, 1 },		SWAP },

    { "2>r",  { 2, 1, 2 },    { 2, 2, 1 },		TEST_TWO_TO_R },
    { "2r>",  { 2, 1, 2 },    { 2, 2, 1 },		TEST_TWO_R_FROM },
    { "2r@",  { 2, 1, 2 },    { 2, 1, 2 },		TEST_TWO_R_FETCH },

    { "rstack", { 2, F_FALSE, F_TRUE}, { 2, F_TRUE, F_TRUE},	TEST_RSTACK },
    { "r@", { 1, 1 }, { 2, 1, 1 },				TEST_RFETCH },

    { "?dup true", { 1, F_TRUE }, { 2, F_TRUE, F_TRUE },	QUESTION_DUP },
    { "?dup false", { 1, F_FALSE }, { 1, F_FALSE },		QUESTION_DUP },

    { "pick", { 4, 1, 2, 3, 2 },     { 4, 1, 2, 3, 1 },		PICK },
    { "roll", { 5, 1, 2, 3, 4, 3 },  { 4, 2, 3, 4, 1 },		ROLL },

    { NULL },
};

static struct test_suite
stackops_suite = { "stackops", stackops_tests };


static bool
results_match(struct fargs *expect, struct fargs *actual)
{
    if (expect->depth != actual->depth) {
	return false;
    }
    for (int i = 0; i < expect->depth; i++) {
	if (expect->stack[i] != actual->stack[i]) {
	    return false;
	}
    }
    return true;
}


static void
print_stack(char *prefix, struct fargs *st)
{
    assert(st->depth <= FARGS_LEN);
    printf("%-10s %lu cells (", prefix, st->depth);
    for (int i = 0; i < st->depth; i++) {
	printf(" %ld", (snumber_ft) st->stack[i]);
    }
    printf(" )\n");
}


static void
run_suite(struct test_suite *suite)
{
    struct test_case *cur_test = suite->test_list;

    printf("%s: ", suite->name);
    while (cur_test->name != NULL) {
	struct fargs args = cur_test->input;
	int except = forth_execute(&args, cur_test->exec_token);
	if (except != 0) {
	    printf("exception %s: %d\n", cur_test->name, except);
	    print_stack("stack:", &args);
	} else if (!results_match(&cur_test->expect, &args)) {
	    printf("mismatch %s:\n", cur_test->name);
	    print_stack("expected:", &cur_test->expect);
	    print_stack("actual:", &args);
	} else {
	    putchar('.');
	}
	cur_test++;
    }
    putchar('\n');
}


int
main(int argc, char *argv[])
{
    run_suite(&execute_suite);
    run_suite(&arithops_suite);
    run_suite(&stackops_suite);
    return EXIT_SUCCESS;
}
