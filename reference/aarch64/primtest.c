/*
 * Copyright 2024, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cforth.h"
#include "prim.h"


#define N(val)		{ .cell = (val) },
#define X(tok)		{ .ip = tok },
#define H(hdlr)		{ .handler = hdlr },
#define L(n)		X(DO_LITERAL) N(n)

#define DEFINITION(nm, hdlr)	vmcode_ft nm[] = { H(hdlr)
#define END_DEF			};
#define CODE(nm)		DEFINITION(nm, do_colon)
#define PRIM(nm, hdlr)		DEFINITION(nm, hdlr) END_DEF
#define END_CODE		X(EXIT) END_DEF


struct test_case {
    char *		name;
    struct fargs	input;
    int			exception;
    struct fargs	expect;
    vmcodeptr_ft	exec_token;
};

struct test_suite {
    char *		name;
    struct test_case *	test_list;
};


/*
 * Tests for stuff in execute.m4
 */

PRIM(DO_LITERAL, do_literal)
PRIM(EXECUTE, x_execute)
PRIM(EXIT, x_exit)
PRIM(DEPTH, x_depth)
PRIM(CLEAR, x_clear)
PRIM(THROW, x_throw)

CODE(NO_OP) END_CODE
CODE(TEST_LIT) L(F_TRUE) END_CODE
DEFINITION(TEST_CON, do_constant) N(F_TRUE) END_DEF
DEFINITION(TEST_VAR, do_variable) N(F_TRUE) END_DEF
CODE(TEST_EXECUTE) L((cell_ft) NO_OP) X(EXECUTE) END_CODE

#define VAR_ADDR	((cell_ft) (&TEST_VAR[1]))

static struct test_case
execute_tests[] = {
    // This test will exercise NEXT, do_colon, and x_exit, because
    // nothing much is useful without them.
    { "no-op", FS0, 0, FS0, NO_OP },

    // This test is meant to exercise argument copy-in/copy-out in
    // forth_execute(), 'cause that'll be wrong the first time you write
    // it for a new CPU.  Probably wrong the second time, too...
    { "true no-op", FS1(F_TRUE), 0, FS1(F_TRUE), NO_OP },

    { "lit", FS0, 0, FS1(F_TRUE), TEST_LIT },
    { "con", FS0, 0, FS1(F_TRUE), TEST_CON },
    { "var", FS0, 0, FS1(VAR_ADDR), TEST_VAR },
    { "execute", FS0, 0, FS0, TEST_EXECUTE },
    { "depth", FS2(0, 1), 0, FS3(0, 1, 2), DEPTH },
    { "clear", FS2(0, 1), 0, FS0, CLEAR },
    { "1 throw", FS1(1), 1, FS0, THROW },
    { "2 1 throw", FS2(2, 1), 1, FS0, THROW },
    { "2 0 throw", FS2(2, 0), 0, FS1(2), THROW },

    // Missing tests that are harder to write:
    //   x_rclear
    //   do_catch
    //   drop_catch
    //   do_create
    //   do_s_quote
    //   do_c_quote
    { NULL },
};


/*
 * Tests for stuff in arithops.m4
 */
PRIM(PLUS, x_plus);
PRIM(MINUS, x_minus);
PRIM(TWO_STAR, x_two_star);
PRIM(TWO_SLASH, x_two_slash);
PRIM(LESS_THAN, x_less_than);
PRIM(EQUALS, x_equals);
PRIM(GREATER_THAN, x_greater_than);
PRIM(AND, x_and);
PRIM(INVERT, x_invert);
PRIM(LSHIFT, x_lshift);
PRIM(NEGATE, x_negate);
PRIM(OR, x_or);
PRIM(RSHIFT, x_rshift);
PRIM(U_LESS, x_u_less);
PRIM(XOR, x_xor);

static struct test_case
arithops_tests[] = {
    { "+",        FS2(1, 2),     0, FS1(3),		PLUS },
    { "-",        FS2(3, 2),     0, FS1(1),		MINUS },
    { "2*",       FS1(3),        0, FS1(6),		TWO_STAR },
    { "2/",       FS1(-2),       0, FS1(-1),		TWO_SLASH },
    { "and",      FS2(0xc, 0xa), 0, FS1(0x8),		AND },
    { "or",       FS2(0xc, 0xa), 0, FS1(0xe),		OR },
    { "xor",      FS2(0xc, 0xa), 0, FS1(0x6),		XOR },

    { "invert",   FS1(F_FALSE),  0, FS1(F_TRUE),	INVERT },
    { "negate",   FS1(1),        0, FS1(-1),		NEGATE },
    { "lshift",   FS2(3, 1),     0, FS1(6),		LSHIFT },
    { "rshift",   FS2(3, 1),     0, FS1(1),		RSHIFT },

    { "< true",   FS2(1, 2),     0, FS1(F_TRUE),	LESS_THAN },
    { "< false",  FS2(2, 1),     0, FS1(F_FALSE),	LESS_THAN },
    { "= true",   FS2(1, 1),     0, FS1(F_TRUE),	EQUALS },
    { "= false",  FS2(1, 2),     0, FS1(F_FALSE),	EQUALS },
    { "> true",   FS2(0, -1),    0, FS1(F_TRUE),	GREATER_THAN },
    { "> false",  FS2(-1, 0),    0, FS1(F_FALSE),	GREATER_THAN },
    { "u< true",  FS2(0, -1),    0, FS1(F_TRUE),	U_LESS },
    { "u< false", FS2(-1, 0),    0, FS1(F_FALSE),	U_LESS },

    { NULL },
};


/*
 * Tests for stuff in stackops.m4
 */
PRIM(TO_R, x_to_r);
PRIM(QUESTION_DUP, x_question_dup);
PRIM(DROP, x_drop);
PRIM(DUP, x_dup);
PRIM(OVER, x_over);
PRIM(R_FROM, x_r_from);
PRIM(R_FETCH, x_r_fetch);
PRIM(ROT, x_rot);
PRIM(SWAP, x_swap);
PRIM(TWO_TO_R, x_two_to_r);
PRIM(TWO_R_FROM, x_two_r_from);
PRIM(TWO_R_FETCH, x_two_r_fetch);
PRIM(PICK, x_pick);
PRIM(ROLL, x_roll);

CODE(TEST_RSTACK) X(TO_R) X(INVERT) X(R_FROM) END_CODE
CODE(TEST_RFETCH) X(TO_R) X(R_FETCH) X(R_FROM) END_CODE
CODE(TEST_TWO_TO_R) X(TWO_TO_R) X(R_FROM) X(R_FROM) END_CODE
CODE(TEST_TWO_R_FROM) X(TO_R) X(TO_R) X(TWO_R_FROM) END_CODE
CODE(TEST_TWO_R_FETCH)
    X(TWO_TO_R) X(TWO_R_FETCH) X(TWO_R_FROM) X(DROP) X(DROP)
END_CODE

static struct test_case
stackops_tests[] = {
    { "drop", FS1(1),       0, FS0,			DROP },
    { "dup",  FS1(1),       0, FS2(1, 1),		DUP },
    { "over", FS2(1, 2),    0, FS3(1, 2, 1),		OVER },
    { "rot",  FS3(1, 2, 3), 0, FS3(2, 3, 1),		ROT },
    { "swap", FS2(1, 2),    0, FS2(2, 1),		SWAP },

    { "2>r",  FS2(1, 2),    0, FS2(2, 1),		TEST_TWO_TO_R },
    { "2r>",  FS2(1, 2),    0, FS2(2, 1),		TEST_TWO_R_FROM },
    { "2r@",  FS2(1, 2),    0, FS2(1, 2),		TEST_TWO_R_FETCH },

    { ">r r>",  FS2(F_FALSE, F_TRUE), 0, FS2(F_TRUE, F_TRUE),	TEST_RSTACK },
    { "r@",     FS1(1),               0, FS2(1, 1),		TEST_RFETCH },

    { "?dup true",  FS1(F_TRUE),  0, FS2(F_TRUE, F_TRUE),	QUESTION_DUP },
    { "?dup false", FS1(F_FALSE), 0, FS1(F_FALSE),		QUESTION_DUP },

    { "pick",   FS4(1, 2, 3, 2),     0, FS4(1, 2, 3, 1),	PICK },
    { "3 roll", FS5(1, 2, 3, 4, 3),  0, FS4(2, 3, 4, 1),	ROLL },
    { "0 roll", FS5(1, 2, 3, 4, 0),  0, FS4(1, 2, 3, 4),	ROLL },

    { NULL },
};


/*
 * Tests for stuff in memops.m4
 */

PRIM(STORE, x_store)
PRIM(FETCH, x_fetch)
PRIM(C_STORE, x_c_store)
PRIM(C_FETCH, x_c_fetch)

CODE(CELL_MEM) L(1) X(TEST_VAR) X(STORE) X(TEST_VAR) X(FETCH) END_CODE
CODE(CHAR_MEM) L('a') X(TEST_VAR) X(C_STORE) X(TEST_VAR) X(C_FETCH) END_CODE

static struct test_case
memops_tests[] = {
    { "! @", FS0, 0, FS1(1),		CELL_MEM },
    { "C! C@", FS0, 0, FS1('a'),	CHAR_MEM },

    // Missing tests that are hard to write:
    //   fill
    //   move
    { NULL },
};


/*
 * Tests for stuff in multops.m4
 */

PRIM(STAR, x_star)
PRIM(SLASH, x_slash)
PRIM(MOD, x_mod)
PRIM(SLASH_MOD, x_slash_mod)
PRIM(M_STAR, x_m_star)
PRIM(U_M_STAR, x_u_m_star)

static struct test_case
multops_tests[] = {
    { "2 3 *",     FS2(2, 3),   0, FS1(6),		STAR },
    { "7 3 /",     FS2(7, 3),   0, FS1(2),		SLASH },
    { "7 3 mod",   FS2(7, 3),   0, FS1(1),		MOD },
    { "7 3 /mod",  FS2(7, 3),   0, FS2(1, 2),		SLASH_MOD },
    { "7 0 /",     FS2(7, 0), -10, FS0,			SLASH },
    { "7 0 mod",   FS2(7, 0), -10, FS0,			MOD },
    { "7 0 /mod",  FS2(7, 0), -10, FS0,			SLASH_MOD },
    { "7 0 /mod",  FS2(7, 0), -10, FS0,			SLASH_MOD },
    { "-1 1 m*",   FS2(-1, 1),  0, FS2(-1, -1),		M_STAR },
    { "-1 -1 m*",  FS2(-1, -1), 0, FS2(1, 0),		M_STAR },
    { "-1 -1 um*", FS2(-1, -1), 0, FS2(1, -2),		U_M_STAR },
    { "-1 2 um*",  FS2(-1, 2),  0, FS2(-2, 1),		U_M_STAR },

    // Missing tests that we aren't yet ready for
    //   M*
    //   UM*
    //   */
    //   */MOD
    //   FM/MOD
    //   SM/REM
    //   UM/MOD
    { NULL },
};


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
run_test(struct test_case *test)
{
    struct fargs args = test->input;
    int except = forth_execute(test->exec_token, &args);
    if (except != test->exception) {
	printf("\nexception %s: ", test->name);
	if (test->exception != 0) {
	    printf("expected %d actual ", test->exception);
	}
	printf("%d\n", except);
	print_stack("stack:", &args);
    } else if (!results_match(&test->expect, &args)) {
	printf("\nmismatch %s:\n", test->name);
	print_stack("expected:", &test->expect);
	print_stack("actual:", &args);
    } else {
	putchar('.');
    }
}


static void
run_suite(struct test_suite *suite)
{
    struct test_case *cur_test = suite->test_list;

    printf("%s: ", suite->name);
    while (cur_test->name != NULL) {
	run_test(cur_test);
	cur_test++;
    }
    putchar('\n');
}


static struct test_suite
all_suites[] = {
    { "execute",   execute_tests },
    { "arithops",  arithops_tests },
    { "stackops",  stackops_tests },
    { "memops",    memops_tests },
    { "multops",   multops_tests },

    { NULL },
};


int
main(int argc, char *argv[])
{
    struct test_suite *cur_suite = all_suites;

    while (cur_suite->name != NULL) {
	run_suite(cur_suite);
	cur_suite++;
    }

    return EXIT_SUCCESS;
}
