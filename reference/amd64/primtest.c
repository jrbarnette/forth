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
    { "+", { 2, 1, 2 }, { 1, 3 }, PLUS },		// ( 1 2 -- 3 )
    { "-", { 2, 3, 2 }, { 1, 1 }, MINUS },		// ( 3 2 -- 1 )
    { "2*", { 1, 3 }, { 1, 6 }, TWO_STAR },		// ( 3 -- 6 )
    { "2/", { 1, -2 }, { 1, -1 }, TWO_SLASH },		// ( -2 -- -1 )
    { "< true", { 2, 1, 2 },
		{ 1, F_TRUE }, LESS_THAN },		// ( 1 2 -- true )
    { "< false", { 2, 2, 1 },
		{ 1, F_FALSE }, LESS_THAN },		// ( 2 1 -- false )
    { "= true", { 2, 1, 1 },
		{ 1, F_TRUE }, EQUALS },		// ( 1 1 -- true )
    { "= false", { 2, 1, 2 },
		{ 1, F_FALSE }, EQUALS },		// ( 1 2 -- false )
    { "> true", { 2, 2, 1 },
		{ 1, F_TRUE }, GREATER_THAN },		// ( 2 1 -- true )
    { "> false", { 2, 1, 2 },
		{ 1, F_FALSE }, GREATER_THAN },		// ( 1 2 -- false )
    { "and", { 2, 0xc, 0xa }, { 1, 0x8 }, AND },	// ( $c $a -- $8 )
    { "invert", { 1, F_FALSE },
		{ 1, F_TRUE }, INVERT },		// ( false -- true )
    { "lshift", { 2, 3, 1 }, { 1, 6 }, LSHIFT },	// ( 3 1 -- 6 )
    { "negate", { 1, 1 }, { 1, -1 }, NEGATE },		// ( -- true )
    { "or", { 2, 0xc, 0xa }, { 1, 0xe }, OR },		// ( $c $a -- $e )
    { "rshift", { 0 }, { 1, F_TRUE }, TEST_LIT },	// ( -- true )
    { "rshift", { 2, 3, 1 }, { 1, 1 }, RSHIFT },	// ( 3 1 -- 1 )
    { "u< true", { 2, 0, -1 },
		{ 1, F_TRUE }, U_LESS },		// ( 0 -1 -- true )
    { "u< false", { 2, -1, 0 },
		{ 1, F_FALSE }, U_LESS },		// ( -1 0 -- false )
    { "xor", { 0 }, { 1, F_TRUE }, TEST_LIT }	,	// ( -- true )
    { "xor", { 2, 0xc, 0xa }, { 1, 0x6 }, XOR },	// ( $c $a -- $6 )
    { NULL },
};

static struct test_suite
arithops_suite = { "arithops", arithops_tests };


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
    return EXIT_SUCCESS;
}
