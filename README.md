A Forth interpreter
===================
This is an implementation of a Forth interpreter in C for a POSIX
environment, targeting conformance with ANSI X3.215-1994 (ANS Forth).

The code has a small handful of dependencies:
  * Consistent with POSIX standard requirements, the C sources
    depend on a standard C compiler supporting the C99 standard or
    later.
  * The C sources depend on the <stdio.h> library for file
    operations, not the POSIX system calls.
  * The C sources depend on POSIX `getopt()`.
  * The C sources depend on GNU `readline()`.
  * The `gen-dict` script depends on a Bourne/POSIX style shell.  This
    script is used to rebuild the dictionary after changing the Forth
    source.

The POSIX dependencies are minor, and should be easy to replace.

The dependency on GNU `readline()` is isolated to a single file, and
should also be easy to replace.

The code is known to build and run on macOS and Linux systems.  I
haven't tested any other target.

The source is available under the terms of the GNU General Public
License, v2.0.  See the LICENSE file.

Purpose
=======
This is done entirely for my own amusement, without any expectation
that it will serve any practical purpose.  Really, I just want to see
what a Forth interpreter looks like when I write it.
