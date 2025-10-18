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
by the builder (see [bootstrap/dictionary.h](bootstrap/dictionary.h)).
It would be nice to get rid of it.  The `lineno` and `input` fields
are used only by primitives, and could just be global variables.
Getting rid of the storage for `HERE` and `FORTH-WORDLIST` is a bit more
work.

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

- `WORD` is supposed to skip leading delimiters.  It doesn't.

- Error handling at initialization is dodgy.  Errors get flagged by
  printing the last source line, with a **^^^** indicator for the token
  at the failure.  But, that goes to `stdout`.  Then, the exception is
  raised, and the exception message goes to `stderr`.
  + ... But this seems to matter only for errors from `interpret_lines()`,
    meaning primarily initialization code in `init_forth_defs`.
  + ... And this problem can be fixed by moving the relevant
    initialization from C to Forth.

- Need better recovery when there's an error while compiling a
  `:` definition.  I think mostly that means roll back the dictionary
  to the point prior to the start of the definition and the partial
  definition. (I think name space isn't modified at that point...)

- We are ready to get rid of `file-order`, by using `INCLUDE-FILE` and
  other code to load directly from source.  (But do we want to?)

- `gen-dict` takes a target name (e.g. `meta`) as an argument.  That was
  useful for experimenting with different meta-compiler strategies, but
  maybe doesn't make sense any more.  At some point, we'll want to
  simplify it a bit.

- Consider making `ONLY` a real Root vocabulary.

- Need to implement `ENVIRONMENT?`

- Need to implement `RECURSE`

- Should implement `VALUE` and `TO`

- Should implement `MARKER`

- Consider implementing the DOUBLE Word Set

- Should finish the FILE Word Set:
  * `DELETE-FILE`
  * `RESIZE-FILE`
  * `S"`  (This is a pain)
  * `FILE-STATUS`  - EXT (What should this even mean?)
  * `FLUSH-FILE`  - EXT
  * `RENAME-FILE`  - EXT
