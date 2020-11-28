A Forth interpreter
===================
This is an implementation of a Forth interpreter in C for a POSIX
environment, targeting conformance with ANSI X3.215-1994 (ANS Forth).

The code has a small handful of dependencies:
  * The C sources depend on a modern standard C compiler.  Anything
    supporting the C99 standard or later should work.
  * The C sources depend on POSIX `getopt()`.
  * The `gen-dict` script depends on a Bourne/POSIX style shell.  This
    script is used to rebuild the dictionary after changing the Forth
    source.

The POSIX dependencies are minor, and could be easily replaced.

The source is available under the terms of the GNU General Public
License, v2.0.  See the LICENSE file.

Purpose
=======
This is done entirely for my own amusement, without any expectation
that it will serve any practical purpose.  Really, I just want to see
what a Forth interpreter looks like when I write it.
