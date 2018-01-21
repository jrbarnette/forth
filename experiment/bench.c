#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bench.h"

/* : test0 [ niter ] literal begin 1 - dup 0= until ; */
static void
test0(cell_ft niter)
{
    label_ft	start;

    LITERAL(niter);
    start = LABEL();
	LITERAL(1); MINUS();
    DUP(); ZEROP(); ZSKIP(start);
}


/* : test1 [ niter ] literal begin nop 1 - dup 0= until ; */
static void
test1(cell_ft niter)
{
    label_ft	start;

    LITERAL(niter);
    start = LABEL();
	NOP(); LITERAL(1); MINUS();
    DUP(); ZEROP(); ZSKIP(start);
}


/* : test2 [ niter ] literal begin dup while 1 - repeat ; */
static void
test2(cell_ft niter)
{
    label_ft	start;
    label_ft	fwd;

    LITERAL(niter);
    start = LABEL();
	DUP(); fwd = LABEL(); ZSKIP(0);
	LITERAL(1); MINUS();
    SKIP(start);
    PATCH(fwd);
}


/* : test3 [ niter ] literal begin dup drop 1 - dup 0= until ; */
static void
test3(cell_ft niter)
{
    label_ft	start;

    LITERAL(niter);
    start = LABEL();
	DUP(); DROP();
	LITERAL(1); MINUS();
    DUP(); ZEROP(); ZSKIP(start);
}


/* : test4 [ niter ] literal begin 1 - dup swap 0= until ; */
static void
test4(cell_ft niter)
{
    label_ft	start;

    LITERAL(niter);
    start = LABEL();
	LITERAL(1); MINUS();
	DUP(); SWAP();
    ZEROP(); ZSKIP(start);
}


/* : test5 [ niter ] literal n ! begin n @ 1 - dup n ! 0= until n @ ; */
static void
test5(cell_ft niter)
{
    label_ft	start;

    LITERAL(niter); ADDR(); STORE();
    start = LABEL();
	ADDR(); FETCH();
	LITERAL(1); MINUS();
	DUP(); ADDR(); STORE();
    ZEROP(); ZSKIP(start);
    ADDR(); FETCH();
}


static void
measure(cell_ft niter, void (*func)(cell_ft), char *name)
{
    time_t	starttime;
    time_t	elapsed;
    unsigned long ninstr;

    compile_init();
    func(niter);
    compile_fini();
    (void) printf("measuring %s\n", name);
    starttime = time(NULL);
    ninstr = interpret();
    elapsed = time(NULL) - starttime;
    (void) printf("   %3lu sec, %9lu inst, %5.2f nsec/inst +/- %.2f\n",
	    elapsed, ninstr, 1e9 * elapsed / ninstr, .5e9 / ninstr);
}


int
main(int argc, char *argv[])
{
    char       *valid = NULL;
    cell_ft	niter = 0;

    if (argc == 2) {
	niter = (cell_ft) strtoul(argv[1], &valid, 10);
    }
    if (valid == NULL) {
	fprintf(stderr, "usage: bench <niter>\n");
	exit(EXIT_FAILURE);
    }
    measure(niter, test0, "begin 1 - dup 0= until");
    measure(niter, test1, "begin nop 1 - dup 0= until");
    measure(niter, test2, "begin dup while 1 - repeat");
    measure(niter, test3, "begin dup drop 1 - dup 0= until");
    measure(niter, test4, "begin 1 - dup swap 0= until");
    measure(niter, test5, "n ! begin n @ 1 - dup n ! 0= until n @");
}
