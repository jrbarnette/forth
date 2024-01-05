# What's it all about?
The purpose here is to eliminate these special direct-mode Forth
primitives:
    i_addname
    i_linkname
    i_reference
    i_setflags
    i_startname
    meta_compile
    meta_interpret

# Structure of the output
Below is the summary of how initdict.c should be structured to reach
the purpose, w/o specific regard for how to generate...

First, drop direct threaded initialization.  It's inconvenient to the
scheme below.  Plus, if we switch to indirect-threaded assembly language
primitives, direct threading will no longer work anyway.

Conceptually, we'd have several arrays.  First off would be a template
dictionary with definitions and name data, but not in the final form.

```
vminstr_ft
dictionary_data[] = {
    // primitives will be a name (as counted string) and handler
    // pointer.
    { .id = "\002>R" },
    { .handler = x_to_r },
    { .id = "\004?DUP" },
    { .handler = x_question_dup },
    // ... more primitives and other data
};

vminstr_ft
builder_definitions[] = {
    // a sequence of unnamed colon definitions referring to addresses
    // in this array, or to primitives

    // this is +! ->
    { .handler = do_colon },
	{ /* dup */      .ip = &dictionary_data[...] },
	{ /* >r */       .ip = &dictionary_data[...] },
	{ /* @ */        .ip = &dictionary_data[...] },
	{ /* + */        .ip = &dictionary_data[...] },
	{ /* r> */       .ip = &dictionary_data[...] },
	{ /* ! */        .ip = &dictionary_data[...] },
	{ /* EXIT */     .ip = &dictionary_data[...] },

    // this is , ->
    { .handler = do_colon },
	{ /* here */     .ip = &builder_definitions[...] },
	{ /* do-literal */ .ip = &dictionary_data[...] },
	{ .cell = (cell_ft) (CELL_SIZE) },
	{ /* allot */    .ip = &builder_definitions[...] },
	{ /* ! */        .ip = &dictionary_data[...] },
	{ /* EXIT */     .ip = &dictionary_data[...] },

    // ... more definitions, culminating in the meta-interpretation
    // primitives outlined below.
};

#define NREFERENCE ...
a_addr_ft reference_table[NREFERENCE] = { NULL };

vminstr_ft
initialize_forth[] = {
    // Executable program drawing on the definitions in the arrays
    // above, as explained below.
};
```

Initialization will start executing (via execute()) the sequence of XTs
found in the initialize_forth array.  The code executed will basically
be the same as the current direct-mode execution, but with only the
instructions; operands will be separated but in the same order in
dictionary_data.

These are the operations needed for initialization:
    do-literal
    meta-startname
    meta-addname
    meta-linkname
    meta-setflags
    meta-interpret
    meta-compile
    meta-reference

Here are the implementations of the substantive pieces:
```
\ `ref-entries` is separate; provides address of ref table
variable ref-index 1 ref-index !

: ref-addr ( index -- a-addr ) cells ref-entries + ;
: ref@ ( index -- xt ) ref-addr @ ;
: ref! ( a-addr -- xt ) ref-addr ! ;
: next-ref! ( name -- ) name>xt ref-index @ ref! 1 ref-index +! ;

\ meta-mode dictionary building operations:
: meta-startname ( a-addr -- name a-addr' )
    dup 2@ count name, dup next-ref! swap 2 cells + ;
: meta-linkname ( name a-addr -- a-addr ) swap link-name ;
: meta-addname ( a-addr -- a-addr' ) meta-startname meta-link-name ;
: meta-setflags ( a-addr -- a-addr' ) dup @ current-name name-flags! cell+ ;

: meta-interpret ( x*i a-addr -- x*j a-addr' )
    begin dup cell+ >r @ ?dup while ref@ execute r> repeat r> ;

: meta-compile ( a-addr -- a-addr' )
    begin dup cell+ swap @ ?dup while ref@ compile, repeat ;

: meta-reference ( a-addr -- ref a-addr' ) dup @ ref@ swap cell+ ;
```
(See reference/forth/meta/meta.fth for more of what's possible)

Initialization code would start out something like this:
    do-literal <C> &dictionary_data[0];
    meta-addname	\ first primitive
    meta-addname	\ second primitive
    \\ eventually, we'd hit a colon defintion, which might look
    \\ something like this:
    meta-startname	\ for colon definition
    meta-compile
    meta-linkname

# How'd we get here?
Switching to 100% indirect seems easier for generation.  Probably.
It definitely spares us any extra primitives (notably, say, stuff
like i_call() or i_strlen()).

Second, the division into three parts is mainly conceptual:  We need
a large block of code holding subroutines; when building the output,
this would have to be buffered, and then output in a block.  And
although the build instructions (initialize_forth) and operands 
(dictionary_data) could be a single stream, as conceived, the
operands are also subroutine data (the primitives).  So, to avoid
a tangle of circular dependencies, make a third part.

# What trouble is still left?
Except for do-literal, all of the initialization primitives depend
on the memory addressed by HERE and/or FORTH-WORDLIST.  If we don't
want to hard-code those locations as "part of the static dictionary",
then we don't know where those locations will be in the final dictionary.

But, if we try and mix and match executing static code and code in the
final dictionary, both side of the code, both sides will need access
at minimum to HERE (for ALLOT and derived operations, and for control
flow branch patching).
