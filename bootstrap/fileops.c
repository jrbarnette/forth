/*
 * Copyright 2011, by J. Richard Barnette
 */

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "forth.h"

/*
 * fileops.c - Standard Forth words for the optional File-Access
 *   word set.
 */

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

/* BIN               11.6.1.0765 FILE, p. 80 */
/* ( fam1 -- fam2 ) */
static vminstr_p
x_bin(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    cell_ft *sp = SP(vm);
    CHECK_POP(vm, 1);
    PICK(sp, 0) |= FILE_BIN;
    return ip;
}

/* CLOSE-FILE        11.6.1.0900 FILE, p. 80 */
/* ( fileid -- ior ) */
static vminstr_p
x_close_file(vminstr_p ip, vmstate_p vm, addr_ft ignore)
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

/* CREATE-FILE       11.6.1.1010 FILE, p. 81 */
/* ( c-addr u fam -- fileid ior ) */
static vminstr_p
x_create_file(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    do_open_create(vm, false);
    return ip;
}

/* FILE-POSITION     11.6.1.1520 FILE, p. 81 */
/* ( fileid -- ud ior ) */
static vminstr_p
x_file_position(vminstr_p ip, vmstate_p vm, addr_ft ignore)
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

/* FILE-SIZE         11.6.1.1522 FILE, p. 81 */
/* ( fileid -- ud ior ) */
static vminstr_p
x_file_size(vminstr_p ip, vmstate_p vm, addr_ft ignore)
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

/* OPEN-FILE         11.6.1.1970 FILE, p. 82 */
/* ( c-addr u fam -- fileid ior ) */
static vminstr_p
x_open_file(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    do_open_create(vm, true);
    return ip;
}

/* R/O               11.6.1.2054 FILE, p. 82 */
/* ( -- fam ) */
static vminstr_p
x_r_o(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, FILE_R_O);
    return ip;
}

/* R/W               11.6.1.2056 FILE, p. 83 */
/* ( -- fam ) */
static vminstr_p
x_r_w(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, FILE_R_W);
    return ip;
}

/* READ-FILE         11.6.1.2080 FILE, p. 83 */
/* ( c-addr u1 fileid -- u2 ior ) */
static vminstr_p
x_read_file(vminstr_p ip, vmstate_p vm, addr_ft ignore)
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

/* REPOSITION-FILE   11.6.1.2142 FILE, p. 84 */
/* ( ud fileid -- ior ) */
static vminstr_p
x_reposition_file(vminstr_p ip, vmstate_p vm, addr_ft ignore)
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

/* W/O               11.6.1.2425 FILE, p. 85 */
/* ( -- fam ) */
static vminstr_p
x_w_o(vminstr_p ip, vmstate_p vm, addr_ft ignore)
{
    CHECK_PUSH(vm, 1);
    PUSH(vm, FILE_W_O);
    return ip;
}

/* WRITE-FILE        11.6.1.2480 FILE, p. 85 */
/* ( c-addr u1 fileid -- ior ) */
static vminstr_p
x_write_file(vminstr_p ip, vmstate_p vm, addr_ft ignore)
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


defn_dt
fileops_defns[] = {
    { define_name, "BIN",              x_bin },
    { define_name, "CLOSE-FILE",       x_close_file },
    { define_name, "CREATE-FILE",      x_create_file },
    { define_name, "FILE-POSITION",    x_file_position },
    { define_name, "REPOSITION-FILE",  x_reposition_file },
    { define_name, "FILE-SIZE",        x_file_size },
    { define_name, "OPEN-FILE",        x_open_file },
    { define_name, "R/O",              x_r_o },
    { define_name, "R/W",              x_r_w },
    { define_name, "READ-FILE",        x_read_file },
    { define_name, "W/O",              x_w_o },
    { define_name, "WRITE-FILE",       x_write_file },
    { NULL }
};

#if 0
    (                    paren                11.6.1.0080 FILE            80
    DELETE-FILE                               11.6.1.1190 FILE            81
    INCLUDE-FILE                              11.6.1.1717 FILE            81
    INCLUDED                                  11.6.1.1718 FILE            82
    READ-LINE                                 11.6.1.2090 FILE            83
    RESIZE-FILE                               11.6.1.2147 FILE            84
    S"                   s-quote              11.6.1.2165 FILE            84
    SOURCE-ID            source-i-d           11.6.1.2218 FILE            84
    WRITE-LINE                                11.6.1.2485 FILE            85
    FILE-STATUS                               11.6.2.1524 FILE EXT        85
    FLUSH-FILE                                11.6.2.1560 FILE EXT        85
    REFILL                                    11.6.2.2125 FILE EXT        86
    RENAME-FILE                               11.6.2.2130 FILE EXT        86
#endif
