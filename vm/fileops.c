/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "forth.h"
#include "fileio.h"

/*
 * fileops.c - Standard Forth words for the optional File-Access
 *   word set.
 */

/*------  ------  ------  ------  ------  ------  ------  ------
 * CLOSE-FILE         11.6.1.0900 FILE                   80
 * CREATE-FILE        11.6.1.1010 FILE                   81
 * FILE-POSITION      11.6.1.1520 FILE                   81
 * FILE-SIZE          11.6.1.1522 FILE                   81
 * OPEN-FILE          11.6.1.1970 FILE                   82
 * READ-FILE          11.6.1.2080 FILE                   83
 * READ-LINE          11.6.1.2090 FILE                   83
 * REPOSITION-FILE    11.6.1.2142 FILE                   84
 * WRITE-FILE         11.6.1.2480 FILE                   85
 * WRITE-LINE         11.6.1.2485 FILE                   85
 *------  ------  ------  ------  ------  ------  ------  ------
 */


/* ( fileid -- ior ) */
PRIM_HDLR(x_close_file)
{
    CHECK_POP(vm, 1);
    cell_ft *sp = SP(vm);
    PICK(sp, 0) = file_close((FILE *) PICK(sp, 0));
    return ip;
}


/* ( c-addr u fam -- fileid ior ) */
PRIM_HDLR(x_create_file)
{
    CHECK_POP(vm, 3);
    cell_ft *sp = SP(vm);

    PICK(sp, 1) = file_open((FILE **) &PICK(sp, 2),
			    PICK(sp, 0) | FILE_MODE_CREATE,
			    (c_addr_ft) PICK(sp, 2),
			    PICK(sp, 1));
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( fileid -- ud ior ) */
PRIM_HDLR(x_file_position)
{
    CHECK_POP(vm, 1);
    CHECK_PUSH(vm, 2);

    dcell_ft fpos;

    cell_ft ior = file_position((FILE *) POP(vm), &fpos);
    PUSH(vm, fpos.lo);
    PUSH(vm, fpos.hi);
    PUSH(vm, ior);

    return ip;
}


/* ( fileid -- ud ior ) */
PRIM_HDLR(x_file_size)
{
    CHECK_POP(vm, 1);
    CHECK_PUSH(vm, 2);

    dcell_ft fsize;

    cell_ft ior = file_size((FILE *) POP(vm), &fsize);
    PUSH(vm, fsize.lo);
    PUSH(vm, fsize.hi);
    PUSH(vm, ior);

    return ip;
}


/* ( c-addr u fam -- fileid ior ) */
PRIM_HDLR(x_open_file)
{
    CHECK_POP(vm, 3);
    cell_ft *sp = SP(vm);

    PICK(sp, 1) = file_open((FILE **) &PICK(sp, 2),
			    PICK(sp, 0) & ~FILE_MODE_CREATE,
			    (c_addr_ft) PICK(sp, 2),
			    PICK(sp, 1));
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( c-addr u1 fileid -- u2 ior ) */
PRIM_HDLR(x_read_file)
{
    CHECK_POP(vm, 3);
    cell_ft *sp = SP(vm);

    PICK(sp, 1) = file_read((FILE *) PICK(sp, 0),
			    (c_addr_ft) PICK(sp, 2),
			    (size_t) PICK(sp, 1),
			    &PICK(sp, 2));
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( c-addr u1 fileid -- u2 flag ior ) */
PRIM_HDLR(x_read_line)
{
    CHECK_POP(vm, 3);
    cell_ft *sp = SP(vm);

    PICK(sp, 0) = file_read_line((FILE *) PICK(sp, 0),
				 (c_addr_ft) PICK(sp, 2),
				 PICK(sp, 1),
				 &PICK(sp, 2), &PICK(sp, 1));

    return ip;
}


/* ( ud fileid -- ior ) */
PRIM_HDLR(x_reposition_file)
{
    CHECK_POP(vm, 3);
    cell_ft *sp = SP(vm);

    PICK(sp, 2) = file_reposition_file((FILE *) PICK(sp, 0),
				       PICK(sp, 1),
				       (long) PICK(sp, 2));
    SET_SP(vm, sp, 2);
    return ip;
}


/* ( c-addr u fileid -- ior ) */
PRIM_HDLR(x_write_file)
{
    CHECK_POP(vm, 3);
    cell_ft *sp = SP(vm);

    PICK(sp, 2) = file_write_file((FILE *) PICK(sp, 0),
				  (c_addr_ft) PICK(sp, 2),
				  PICK(sp, 1));
    SET_SP(vm, sp, 2);
    return ip;
}


/* ( c-addr u fileid -- ior ) */
PRIM_HDLR(x_write_line)
{
    CHECK_POP(vm, 3);
    cell_ft *sp = SP(vm);

    PICK(sp, 2) = file_write_line((FILE *) PICK(sp, 0),
				  (c_addr_ft) PICK(sp, 2),
				  PICK(sp, 1));
    SET_SP(vm, sp, 2);
    return ip;
}
