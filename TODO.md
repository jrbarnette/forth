## Design/Implementation Refinements
### Where we are
All Forth code now lives under `forth/`, and gets built (one way or
another) into `initdict.c`.  This includes the source code built
into the interpreter for interpretation at startup.

`QUIT` has an implementation in the source text that we interpret at
startup, and the static dictionary is gone, except for `HERE` and
`FORTH-WORDLIST`. So, at this point, almost everything that might
plausibly be in Forth in the target dictionary is now in Forth.  The
only C code left to eliminate is the stuff used for direct mode and
meta-interpretation during dictionary construction at startup.

There's a sample of how replace things like `i_startname()` in
`bootstrap/direct_test.c`.  For that to work, we need control flow
support in DIRECT.  That means we need to resurrect/appropriate the
rawdict code for DIRECT.

### Plan to move forward
The long-term goal is to get rid of C code that isn't a Forth
primitive or the interpreter loop.  Preserving both the indirect-
and direct-threaded loops is OK, and likely useful/necessary.
Simplifying the generation of direct interpreted code in `initdict.c`
would be nice, but not required.

In practice, we want to eliminate these C primitives:
  * `i_addname`
  * `i_linkname`
  * `i_lookup`
  * `i_setflags`
  * `i_startname`
  * `meta_compile`
  * `meta_interpret`

Additionally, we'll want Forth code for most of the system startup
stuff called from `main()` (in `forth.c`).

All told, that means eliminating/replacing the content in these files:
  * `allot.c`
  * `names.c`
  * `meta.c`
  * `forth.c`

It's not much content: ~368 lines, including comments and some
content in `forth.c` that won't be converted.  But... It'd all have to
be implemented as direct-mode code, so we'd need to add `i_call()`.
That's nominally not a big deal...
```
    PRIM_HDLR(i_call)
    {
	CHECK_RPUSH(vm, 1);
	RPUSH(vm, ip + 1);
	return ip->ip;
    }
```

But... we'd also have to implement a lot of infrastructure around
`i_call()` and the extra code would be taking up space in `initdict.c`.
Also, the extra stuff would still represent code duplication, at
least in code space (and possibly as source code?).

So, the plan is to go with the 'rawdict' approach: build up a
dictionary image at compile time, then at startup just `MOVE` the
data to the dictionary storage.

## New Features/Bugs

- Error handling is dodgy.  Errors get flagged by printing the last
  source line, with a **^^^** indicator for the token at the failure.
  But, that goes to `stdout`.  Then, the exception is raised, and the
  exception message goes to `stderr`.

- Need better recovery when there's an error while compiling a
  `:` definition.  I think mostly that means roll back the dictionary
  to the point prior to the start of the definition and the partial
  definition. (I think name space isn't modified at that point...)

- We are ready to get rid of `file-order`, by using `INCLUDE-FILE` and
  other code to load directly from source.  (But do we want to?)

- Need to implement `ENVIRONMENT?`

- Need to implement `RECURSE`

- Should implement `PAD`

- Should implement `CASE` `OF` `ENDOF` `ENDCASE`

- Should implement `VALUE` `TO`

- Should implement `MARKER`
