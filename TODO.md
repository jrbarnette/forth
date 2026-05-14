## Design/Implementation Refinements
### Where we are
There's now a new meta-compiler, living under [forth/meta](forth/meta).
Dependencies on C code are now just command-line processing,
certain parts of initialization, and the necessary C primitive
functions.

### Where to go next
#### Assembly primitives
There are experimental versions in assembly for amd64 in
[reference/amd64](reference/amd64/).  Similar code for ARM aarch64 is
in [reference/aarch64](reference/aarch64/).

We've converted [bootstrap/forth.c](bootstrap/forth.c) to use
`forth_execute()`, so that we can choose (at compile time) what
VM implementation we want.

To get to a working VM with primitives in assembly, all that's left
is (probably mostly) to adjust the build system to support multiple
targets, and add the assembly primitives as a second target.  We should
also extend the build to support building the `primtest` code that is
used to test the assembly primitives.

#### Cleanup
The dictionary still has some data that's laid out statically, not
by the builder (see [bootstrap/initdict.c](bootstrap/initdict.c)).
It would be nice to get rid of it.

When starting up an interactive interpreter session, we run a small
snippet to print dictionary size statistics.  We should move the source
of that code snippet into someplace under [forth](forth/), and build it
into the interpreter with the meta-compiler.  The choice of whether to
invoke it should stay with C code, for now.

Command-line processing and related initialization are still all
in C.  Some of it could be Forth.  Ideally, we'd only depend on
C to invoke `getopt()`, and then pass the information to some sort
of `MAIN` routine in Forth to walk through file arguments and such.

#### Design improvements
There's room now to re-implement primitives from C to assembly.  Starter
code for AMD64 is under [reference/amd64](reference/amd64).  Notes on
VM design for AMD64 and ARM32 are in
[doc/asm-x86-amd64.txt](doc/asm-x86-amd64.txt) and
[doc/asm-arm-a32.txt](doc/asm-arm-a32.txt), respectively.

## New Features/Bugs/Flaws

- The initialization code relating to the terminal is a bit disjointed.
  We need a real interface that allows configuration of the terminal
  input in one of two specific ways:
  + Open an ordinary file by filename.  `is_interactive` is false.
  + Use stdin.  `is_interactive` depends on the combination of the
    command line option and `isatty()`.

- `WORD` is supposed to skip leading delimiters.  It doesn't.

- Error handling at initialization is dodgy.  Errors get flagged by
  printing the last source line, with a **^^^** indicator for the token
  at the failure.  But, that goes to `stdout`.  Then, the exception is
  raised, and the exception message goes to `stderr`.
  + This only happens if there's an exception during initialization.
  + The problem might reasonably be fixed by moving the relevant
    initialization from C to Forth.

- Need better recovery when there's an error while compiling a
  `:` definition.  I think mostly that means roll back the dictionary
  to the point prior to the start of the definition and the partial
  definition. (I think name space isn't modified at that point...)
    + Consider how this is similar to `MARKER` (and `FORGET`).
    + Also, there's some tie-in to RECURSE, see
      [doc/colon-reset.txt](doc/colon-reset.txt)

- We are ready to get rid of `file-order`, by using `INCLUDE-FILE` and
  other code to load directly from source.  (But do we want to?)

- Consider making `ONLY` a real Root vocabulary.

- Missing standard definitions from CORE:
  + `ENVIRONMENT?`
  + `RECURSE`

- Missing standard definitions from CORE EXT:
  + `VALUE` and `TO`
  + `MARKER`

- Missing standard definitions from SEARCH EXT:
  + `ORDER`

- Should finish the FILE Word Set:
  * `DELETE-FILE`
  * `RESIZE-FILE`
  * `S"`  (This is a pain)
  * `FILE-STATUS`  - EXT (What should this even mean?)
  * `FLUSH-FILE`  - EXT
  * `RENAME-FILE`  - EXT

- Consider implementing the DOUBLE Word Set

- Consider implementing (more of) the TOOLS Word Set
