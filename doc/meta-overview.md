# Inventory of notes files:
  * directasm.txt - Ideas relating to an assembler based on direct
	executed programs.
  * meta-notes.txt - Implementation notes for meta-interpretation
	under forth/direct; possibly superseded by the
	implementation.
  * meta-special.txt - Categorization of words needing special treatment
	in meta-compilation, generally.
  * rawdict.txt - Implementation and design notes relating to
	dictionary generation via buffer.
  * target-dict.txt - An initial attempt to scope out implementation
	details and strategy options for a meta-compiler based on
	direct execution.

# This file
Outline of implementation choices, strategies, and trade-offs for the
meta-compiler that builds the Forth image.

# Topics
## Modes of operation
Pure Forth - Forth code not meant to affect (directly) meta-compiled
output.  Used (at minimum) to construct definitions to implement the
other modes.

Host mode - Forth code meant to directly create or manipulate
meta-compiled output.  The code knows the difference between the local
host interpreter environment and the target interpreter environment.
STATE can be either compiled or interpreted.

Target interpretation - Code meant to behave as if it were executed on
the target in interpretation state.

Target compilation - Code meant to compile content to the target as if
it were in compilation state.

## Strategies
Meta-interpretation - Target interpretation code is compiled to the
target and executed on the target to build the dictionary in place.
This is the current strategy in `bootstrap/initdict.c`.  This strategy
allows printing output as it is parsed.

Compile to buffer - target content is accumulated in a buffer, and
the buffer is written out after all processing.  This allows for
backpatching content, as for `IF ... ELSE ... THEN`.  Accumulated data
must generally be tagged with a content type that indicates how it will
be output.

## Vocabularies
