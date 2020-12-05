#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// These must be in a separate file, or the compiler (bless its
// little silicon heart) may inline code them, reducing the entire
// iteration loop in measure() to a no-op.

extern unsigned cdiv(unsigned, unsigned);
extern unsigned cmod(unsigned, unsigned);
extern unsigned divmod(unsigned, unsigned);

static void
measure(unsigned long niter, unsigned (*func)(unsigned, unsigned), char *name)
{
    time_t	starttime;
    time_t	elapsed;

    (void) printf("measuring %s\n", name);
    starttime = time(NULL);
    for (unsigned long i = 0; i < niter; i++) {
	(void) func(7, 3);
    }
    elapsed = time(NULL) - starttime;
    (void) printf("   %3lu sec, %5.2f nsec/iter +/- %.2f\n",
	    elapsed, 1.0e9 * elapsed / niter, 0.5e9 / niter);
}


int
main(int argc, char *argv[])
{
    char	       *valid = NULL;
    unsigned long	niter = 0;

    if (argc == 2) {
	niter = strtoul(argv[1], &valid, 10);
    }
    if (valid == NULL) {
	(void) fprintf(stderr, "usage: bench <niter>\n");
	exit(EXIT_FAILURE);
    }
    (void) printf("niter = %20lu\n", niter);
    measure(niter, cdiv, "div");
    measure(niter, cmod, "mod");
    measure(niter, divmod, "div+mod");
    return EXIT_SUCCESS;
}
