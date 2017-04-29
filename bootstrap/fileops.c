/*
 * Copyright 2013, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "forth.h"
#include "direct.h"

/*
 * fileops.c - Standard Forth words for the optional File-Access
 *   word set.
 */

//------  ------  ------  ------  ------  ------  ------  ------
// BIN                11.6.1.0765 FILE                   80
// CLOSE-FILE         11.6.1.0900 FILE                   80
// CREATE-FILE        11.6.1.1010 FILE                   81
// FILE-POSITION      11.6.1.1520 FILE                   81
// FILE-SIZE          11.6.1.1522 FILE                   81
// OPEN-FILE          11.6.1.1970 FILE                   82
// R/O                11.6.1.2054 FILE                   82
// R/W                11.6.1.2056 FILE                   83
// READ-FILE          11.6.1.2080 FILE                   83
// REPOSITION-FILE    11.6.1.2142 FILE                   84
// W/O                11.6.1.2425 FILE                   85
// WRITE-FILE         11.6.1.2480 FILE                   85
//------  ------  ------  ------  ------  ------  ------  ------


#define FILE_CREATE	1
#define FILE_R_O	((cell_ft) 01)
#define FILE_R_W	((cell_ft) 02)
#define FILE_W_O	((cell_ft) 03)
#define FILE_BIN	((cell_ft) 04)

/*
 * Mapping Forth "file access methods" to modes for C fopen():
 *  R/O     OPEN-FILE   -> "r"
 *  R/O BIN OPEN-FILE   -> "rb"
 *  R/W     OPEN-FILE   -> "r+"
 *  R/W BIN OPEN-FILE   -> "r+b"
 *  W/O     OPEN-FILE   -> "r+"
 *  W/O BIN OPEN-FILE   -> "r+b"
 *  R/O     CREATE-FILE -> "w+"
 *  R/O BIN CREATE-FILE -> "w+b"
 *  R/W     CREATE-FILE -> "w+"
 *  R/W BIN CREATE-FILE -> "w+b"
 *  W/O     CREATE-FILE -> "w"
 *  W/O BIN CREATE-FILE -> "wb"
 */
static void
do_open_create(vmstate_p vm, bool is_open)
{
    char filename[256];
    cell_ft fam, len;
    c_addr_ft str;
    cell_ft *sp = SP(vm);

    assert(sizeof (*str) == sizeof (*filename));
    CHECK_POP(vm, 3);
    fam = PICK(sp, 0);
    len = PICK(sp, 1);
    str = (c_addr_ft) PICK(sp, 2);

    if (len < sizeof (filename)) {
	cell_ft mode_flag;
	char fmode[4], *cp = fmode;
	FILE *fp;

	memcpy(filename, str, len);
	filename[len] = '\0';

	if (is_open) {
	    *cp++ = 'r';
	    mode_flag = FILE_R_O;
	} else {
	    *cp++ = 'w';
	    mode_flag = FILE_W_O;
	}
	if ((fam & ~FILE_BIN) != mode_flag) {
	    *cp++ = '+';
	}

	if ((fam & FILE_BIN) != 0) {
	    *cp++ = 'b';
	}
	*cp++ = '\0';
	fp = fopen(filename, fmode);
	if (fp == NULL) {
	    PICK(sp, 1) = errno;
	} else {
	    PICK(sp, 1) = 0;
	}
	PICK(sp, 2) = (cell_ft) fp;
    } else {
	PICK(sp, 1) = ENAMETOOLONG;
	PICK(sp, 2) = (cell_ft) NULL;
    }

    SET_SP(vm, sp, 1);
}


/* ( fam1 -- fam2 ) */
vminstr_p
x_bin(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) |= FILE_BIN;
    return ip;
}


/* ( fileid -- ior ) */
vminstr_p
x_close_file(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    FILE *fp;
    CHECK_POP(vm, 1);
    fp = (FILE *) PICK(sp, 0);
    if (fclose(fp) == 0) {
	PICK(sp, 0) = 0;
    } else {
	PICK(sp, 0) = (cell_ft) errno;
    }
    return ip;
}


/* ( c-addr u fam -- fileid ior ) */
vminstr_p
x_create_file(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    do_open_create(vm, false);
    return ip;
}


/* ( fileid -- ud ior ) */
vminstr_p
x_file_position(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    FILE *fp;
    long fpos;
    CHECK_POP(vm, 1);
    CHECK_PUSH(vm, 2);
    fp = (FILE *) PICK(sp, 0);
    fpos = ftell(fp);
    PICK(sp, 0) = (cell_ft) fpos;
    // FIXME: FILE-POSITION restricted to single cell result
    PICK(sp, -1) = (cell_ft) 0;
    if (fpos >= 0) {
	PICK(sp, -2) = 0;
    } else {
	PICK(sp, -2) = errno;
    }
    SET_SP(vm, sp, -2);
    return ip;
}


/* ( fileid -- ud ior ) */
vminstr_p
x_file_size(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    FILE *fp;
    long fpos, fsize = -1;
    int size_err;

    CHECK_POP(vm, 1);
    CHECK_PUSH(vm, 2);
    fp = (FILE *) PICK(sp, 0);
    fpos = ftell(fp);
    if (fpos >= 0) {
	size_err = fseek(fp, 0L, SEEK_END);
	if (size_err == 0) {
	    fsize = ftell(fp);
	    if (fsize >= 0) {
		size_err = fseek(fp, fpos, SEEK_SET);
	    } else {
		size_err = ferror(fp);
		clearerr(fp);
		(void) fseek(fp, fpos, SEEK_SET);
	    }
	}
    } else {
	size_err = ferror(fp);
	clearerr(fp);
    }
    PICK(sp, 0) = (cell_ft) fpos;
    // FIXME: FILE-SIZE restricted to single cell result
    PICK(sp, -1) = (cell_ft) 0;
    PICK(sp, -2) = (cell_ft) size_err;
    SET_SP(vm, sp, -2);
    return ip;
}


/* ( c-addr u fam -- fileid ior ) */
vminstr_p
x_open_file(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    do_open_create(vm, true);
    return ip;
}


/* ( -- fam ) */
vminstr_p
x_r_o(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, FILE_R_O);
    return ip;
}


/* ( -- fam ) */
vminstr_p
x_r_w(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, FILE_R_W);
    return ip;
}


/* ( c-addr u1 fileid -- u2 ior ) */
vminstr_p
x_read_file(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    FILE *fp;
    size_t nrequest;
    c_addr_ft buff;
    size_t nresponse;

    CHECK_POP(vm, 3);

    fp = (FILE *) PICK(sp, 0);
    nrequest = (size_t) PICK(sp, 1);
    buff = (c_addr_ft) PICK(sp, 2);

    nresponse = fread(buff, sizeof (*buff), nrequest, fp);
    if (nresponse == nrequest) {
	PICK(sp, 1) = 0;
    } else {
	if (feof(fp)) {
	    PICK(sp, 1) = 0;
	} else {
	    PICK(sp, 1) = ferror(fp);
	}
	clearerr(fp);
    }
    PICK(sp, 2) = nresponse;
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( ud fileid -- ior ) */
vminstr_p
x_reposition_file(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    FILE *fp;
    long fpos;

    CHECK_POP(vm, 3);
    fp = (FILE *) PICK(sp, 0);
    fpos = (long) PICK(sp, 2);
    // FIXME:  REPOSITION-FILE when sizeof (ud) != sizeof (long)
    if (PICK(sp, 1) == 0) {
	PICK(sp, 2) = (cell_ft) fseek(fp, fpos, SEEK_SET);
    } else {
	PICK(sp, 2) = (cell_ft) EOVERFLOW;
    }
    SET_SP(vm, sp, 2);
    return ip;
}


/* ( -- fam ) */
vminstr_p
x_w_o(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, FILE_W_O);
    return ip;
}


/* ( c-addr u1 fileid -- ior ) */
vminstr_p
x_write_file(vminstr_p ip, vmstate_p vm, vmarg_p ignore)
{
    cell_ft *sp = SP(vm);
    FILE *fp;
    size_t nrequest;
    c_addr_ft buff;
    size_t nresponse;

    CHECK_POP(vm, 3);

    fp = (FILE *) PICK(sp, 0);
    nrequest = (size_t) PICK(sp, 1);
    buff = (c_addr_ft) PICK(sp, 2);

    nresponse = fwrite(buff, sizeof (*buff), nrequest, fp);
    if (nresponse == nrequest) {
	PICK(sp, 2) = 0;
    } else {
	PICK(sp, 2) = ferror(fp);
	clearerr(fp);
    }
    SET_SP(vm, sp, 2);
    return ip;
}
