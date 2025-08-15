## Design/Implementation Refinements
### Where we are
There's now a new meta-compiler, living under [forth/meta](forth/meta).
Dependencies on C code are now just command-line processing,
certain parts of initialization, and the necessary C primitive
functions.

### Where to go next
#### Assembly primitives
There are experimental versions in assembly for amd64 in
[reference/amd64](reference/amd64/).

To get to a working set of primitives in assembly, we need the following
changes (roughly in this order):
  * Convert `bootstrap` to use the new `forth_execute()` interface in
    place of the current interface.  The initial implementation should
    be a wrapper around the existing C primitves.
  * Adjust the build system to support multiple targets, and add the
    assembly primitives as a second target

First problem: In [bootstrap/forth.c](bootstrap/forth.c), if we do a
simple conversion that changes calls to `execute()` into calls to
`forth_execute()`, then the mini-stack in `struct fargs` overflows
while interpreting the Forth source text in `init_forth_defs`.
The issue is that when creating the table of exception strings, we
build up the table content on the stack before populating the table.

The preferred solution is to move processing of all that source text
into Forth code embedded as C data in
[bootstrap/initdict.c](bootstrap/initdict.c).  There are at least two
other options:
  * Allow a variable length stack in `forth_execute()`.  But the
    caller in `initialize_dictionary()` would have to know about
    the extra stack requirements.
  * Change how we build the error table, so that it lives inside a
    constant size stack.

ATM, I think the easiest way to get a handle on the necessary 
improvements is to implement some of the cleanup described in the
next section.

#### Cleanup
The meta-compiler still has rough edges, mostly relating to
niceties of design and organization, outlined below.

Some of the names in [forth/meta/host.fth](forth/meta/host.fth)
still use the prefix `direct-`.  This isn't the right naming
convention any more.  Lots of other naming conventions need
revisiting, too.

The meta-compiler is stable enough to justify spending time on
documentation.  Some time spent here might help produce clarity
in terminology and naming conventions, too.  My thinking on
terminology is in [doc/final-meta/vocab.txt](doc/final-meta/vocab.txt)

We want the `METACOMPILER` vocabulary to be "everything needed to
produce simple C code forms in the output".  However, currently
`METADICT-HOST` contains private versions that are customized to its
needs.  Ideally, we'd find ways to unify the various output modes.  In
particular, it would be nice to be able to invoke some of the
definitions from `META-TARGET` in `TARGET` code rather than creating
`#define` values for them in C source.

The `TARGET` code in [forth/c-gen/core/literal.fth](forth/c-gen/core/literal.fth)
is complicated. It would be nice if it looked more like
[forth/c-gen/core/allot.fth](forth/c-gen/core/allot.fth).
That would require using more of the `META-TARGET` code, like
`ALLOT` and friends, and compiling `LITERAL` on the host rather
than waiting for the builder to invoke it from the dictionary.

At this point, the names of all definitions that go into
the builder code on the target are known (indirectly) on the target
at meta-compilation time (they're in the `TARGET-NAMES` wordlist).
So, in meta-compiler target code, we could look up target names in
`INTERPRET-NAME`, rather than leaving it to `INTERPRET-UNKNOWN`.
That would mean we'd make all target definitions be executable code
that outputs the needed forms.  This could simplify the code
(maybe?), although the lesson from `METADICT-TARGET` is that the
approach needs several (3-4) separate vocabularies to keep it all
straight.

The dictionary still has some data that's laid out statically, not
by the builder.  It would be nice to get rid of it.  The file
related stuff is used by command-line processing, and could just be
global variables.  Getting rid of the storage for `HERE` and
`FORTH-WORDLIST` is a bit more work.

Command-line processing and related initialization are still all
in C.  A lot of it could be Forth.  Ideally, we'd only depend on
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

- `THROW` is supposed to restore the previous input source
  specification.  It doesn't.

- Related to the problem with `THROW`: Are `SAVE-INPUT` and
  `RESTORE-INPUT` done right?  Need to study the definition of
  "input source specification", and read the fine print in the
  specifications in 6.2.2148 and 6.2.2182.

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
