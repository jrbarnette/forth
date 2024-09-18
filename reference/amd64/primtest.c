/*
 * Copyright 2024, by J. Richard Barnette. All Rights Reserved.
 */

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

DEF_PRIM(EXIT, x_exit);
DEF_PRIM(EXECUTE, x_execute);
DEF_PRIM(DO_LITERAL, do_literal);


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


CODE(NO_OP) END_CODE;

static struct test_case
execute_tests[] = {
    { "no-op", { 0 }, { 0 }, NO_OP, },		// ( -- )
    { NULL },
};

static struct test_suite
execute_suite = { "execute", execute_tests };


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
    printf("%-10s %lu cells (", st->depth, prefix);
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
    return EXIT_SUCCESS;
}
