This is a benchmark experiment to pick the fastest interface to the
C code primitives in the forth interpreter.

Ultimately, the choice was between two interpretation loops.

Option 1 - pass and return top-of-stack to primitives:

    cell_ft tos;
    // ... initialize top-of-stack
    while (vm->ip != NULL) {
        xt_ft xtok = *vm->ip++;
        tos = xtok->handler(tos, vm, xtok[1].data);
    }

Option 2 - pass and return the virtual IP to primitives:

    xt_ft *ip;
    // ... initialize the IP
    while (ip != NULL) {
        xt_ft xtok = *ip++;
        ip = xtok->handler(ip, vm, xtok[1].data);
    }

Option 2 is what's used in the current source.  It won because it's
unambiguously faster on RISC processors and is faster for CISC
(ia32/amd64) in at least some cases.

Where "tos" is faster on CISC:
  * 1- and 2-argument arithmetic operators, like `+` and `0=`.
  * Stack intensive operations like `DUP` and `SWAP`.

Where "ip" is faster on CISC:
  * Invoking :-definitions.
  * Branching and looping primitives.
  * Operations that are less stack-intensive, such as `do_variable()`,
    `@` and `!`.

Cases I've tested at one time or another:
  * 64-bit SPARC (Solaris)
  * 32-bit PPC (Mac OS X)
  * IA32 (Linux)
  * IA32 (Mac OS X)
  * AMD64 (Mac OS X)
  * 32-bit ARM (Raspberry Pi - Linux)
