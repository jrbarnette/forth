## Design/Implementation Refinements
### Where we are
All Forth code now lives under `forth/`, and gets built (one way or
another) into `initdict.c`.  This includes source code built into the
interpreter for interpretation at startup.

`QUIT` has an implementation in the source text that we interpret at
startup, and the static dictionary is gone, except for `HERE` and
`FORTH-WORDLIST`. So, at this point, almost everything that might
plausibly be in Forth in the target dictionary is now in Forth.  The
only C code left to eliminate is the stuff used for direct mode and
meta-interpretation during dictionary construction at startup.

### Plan to move forward
The long-term goal is to get rid of C code that isn't a Forth
primitive or the interpreter loop.  Preserving both the indirect-
and direct-threaded loops is OK, and maybe useful/necessary.
Simplifying the generation of direct interpreted code in `initdict.c`
would be nice, but not required.

In practice, we want to eliminate these C primitives:
  * `i_addname`
  * `i_linkname`
  * `i_setflags`
  * `i_startname`
  * `i_reference`
  * `meta_compile`
  * `meta_interpret`

Additionally, we'll want Forth code for most of the system startup
stuff called from `main()` (in `forth.c`).

All told, that means eliminating/replacing the content in these files:
  * `allot.c`
  * `names.c`
  * `meta.c`
  * `forth.c`

#### The `direct` option
The content to be replaced isn't much: ~368 lines, including comments
and some content in `forth.c` that won't be converted.  But... It'd all
have to be implemented as direct-mode code, so we'd need to add
`i_call()`.  That's nominally not a big deal...
```
    PRIM_HDLR(i_call)
    {
	CHECK_RPUSH(vm, 1);
	RPUSH(vm, ip + 1);
	return ip->ip;
    }
```

But... we'd also have to implement several new things:
  * New direct code invoked via `i_call()` in `initdict.c` to replace
    the existing C functions.
  * New support infrastructure to generate the new direct code.

That could turn out to be large and complicated, and could create its
own code duplication...

#### The `rawdict` option
The complexity of staying `direct` argues for the `rawdict` approach:
build up a dictionary image at compile time, then at startup just `MOVE`
the data to the dictionary storage.  The problem is that this means
`initdict.c` is tied to a particular cell size.  I don't like that...

So, I'm looking at a hybrid approach to `rawdict`.  The dictionary would
be an arrary of entries like this:
```
    union {
	cell_ft         cell;
	c_addr_ft       str;
	cell_ft         ref;
	vminstr_fn      handler;
    } raw_dictionary[] = {
	...
    };
```

Rather than populating the dictionary with a raw `memcpy()`, each entry
in `raw_dictionary` would be processed along these lines:
  * A `cell` or `handler` entry is appended directly to the next cell in
    the dictionary.
  * The bytes pointed to by a `str` entry are appended to the
    dictionary.
  * A `ref` entry holds the index of an earlier entry in the
    `raw_dictionary` array: append to the dictionary the address where
    that entry was copied to in the dictionary.

This implies we'd need a secondary array of relocation addresses for
`ref` entries. In theory we could reuse the slot in `raw_dictionary`,
but that makes me itchy, so maybe not...

Additionally, `rawdict` would have to include a tag for each entry in
`raw_dictionary` so the copy code would know how to process the entry.

This hybrid approach would also need some special processing for cells
with forward references, namely `HERE` and `FORTH-WORDLIST`.

#### Obvious next steps
Fix known bugs, and implement missing standard features.  Basically,
bring the `bootstrap` interpreter up to full compliance with CORE,
CORE EXT, and a few selected other word sets (especially EXCEPTION,
FILE, TOOLS and SEARCH).

Also, think about possible hybrid solutions combining `direct` with
`rawdict`.

## New Features/Bugs

- WORD is supposed to skip leading delimiters.  It doesn't.

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

- Should implement `VALUE` `TO`

- Should implement `MARKER`
