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

/*
 * fileops.c - Standard Forth words for the optional File-Access
 *   word set.
 */

/*------  ------  ------  ------  ------  ------  ------  ------
 * BIN                11.6.1.0765 FILE                   80
 * CLOSE-FILE         11.6.1.0900 FILE                   80
 * CREATE-FILE        11.6.1.1010 FILE                   81
 * FILE-POSITION      11.6.1.1520 FILE                   81
 * FILE-SIZE          11.6.1.1522 FILE                   81
 * OPEN-FILE          11.6.1.1970 FILE                   82
 * R/O                11.6.1.2054 FILE                   82
 * R/W                11.6.1.2056 FILE                   83
 * READ-FILE          11.6.1.2080 FILE                   83
 * READ-LINE          11.6.1.2090 FILE                   83
 * REPOSITION-FILE    11.6.1.2142 FILE                   84
 * W/O                11.6.1.2425 FILE                   85
 * WRITE-FILE         11.6.1.2480 FILE                   85
 * WRITE-LINE         11.6.1.2485 FILE                   85
 *------  ------  ------  ------  ------  ------  ------  ------
 */


#define UNKNOWN_ERROR   ((cell_ft) -1)

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
 *
 * N.B. In some cases, the file may be opened for operations that
 * weren't requested:
 *     W/O OPEN-FILE opens the file for reading as well.
 *     R/O CREATE-FILE opens the file for writing as well.
 */
static void
do_open_create(vmstate_ft *vm, bool is_open)
{
    char filename[256];
    cell_ft fam, len;
    c_addr_ft str;
    cell_ft *sp = SP(vm);

    assert(sizeof (*str) == sizeof (*filename));
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
	    mode_flag = FILE_MODE_R_O;
	} else {
	    *cp++ = 'w';
	    mode_flag = FILE_MODE_W_O;
	}
	if ((fam & ~FILE_MODE_BIN) != mode_flag) {
	    *cp++ = '+';
	}

	if ((fam & FILE_MODE_BIN) != 0) {
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


/* ( fileid -- ior ) */
PRIM_HDLR(x_close_file)
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
PRIM_HDLR(x_create_file)
{
    CHECK_POP(vm, 3);
    do_open_create(vm, false);
    return ip;
}


/* ( fileid -- ud ior ) */
PRIM_HDLR(x_file_position)
{
    cell_ft *sp = SP(vm);
    FILE *fp;
    long fpos;
    CHECK_POP(vm, 1);
    CHECK_PUSH(vm, 2);
    fp = (FILE *) PICK(sp, 0);
    fpos = ftell(fp);
    // FIXME: FILE-POSITION restricted to single cell result
    PICK(sp, 0) = (cell_ft) fpos;
    PICK(sp, -1) = (cell_ft) 0;
    if (fpos >= 0) {
	PICK(sp, -2) = 0;
    } else {
	PICK(sp, -2) = errno;
	clearerr(fp);
    }
    SET_SP(vm, sp, -2);
    return ip;
}


/* ( fileid -- ud ior ) */
PRIM_HDLR(x_file_size)
{
    CHECK_POP(vm, 1);
    CHECK_PUSH(vm, 1);
    cell_ft *sp = SP(vm);
    FILE *fp = (FILE *) PICK(sp, 0);

    long fsize = 0;
    int ior = 0;
    long save_pos = ftell(fp);
    if (save_pos >= 0) {
	if (fseek(fp, 0L, SEEK_END) >= 0) {
	    fsize = ftell(fp);
	    if (fsize >= 0) {
		if (fseek(fp, save_pos, SEEK_SET) < 0) {
		    ior = errno;
		}
	    } else {
		ior = errno;
		(void) fseek(fp, save_pos, SEEK_SET);
	    }
	} else {
	    ior = errno;
	}
    } else {
	ior = errno;
    }
    if (ior) {
	clearerr(fp);
    }
    // FIXME: FILE-SIZE restricted to single cell result
    PICK(sp, 0) = (cell_ft) fsize;
    PICK(sp, -1) = (cell_ft) 0;
    PICK(sp, -2) = (cell_ft) ior;
    SET_SP(vm, sp, -2);
    return ip;
}


/* ( c-addr u fam -- fileid ior ) */
PRIM_HDLR(x_open_file)
{
    CHECK_POP(vm, 3);
    do_open_create(vm, true);
    return ip;
}


/* ( c-addr u1 fileid -- u2 ior ) */
PRIM_HDLR(x_read_file)
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
	} else if (ferror(fp)) {
	    PICK(sp, 1) = errno;
	    clearerr(fp);
	} else {
	    PICK(sp, 1) = UNKNOWN_ERROR;
	}
    }
    PICK(sp, 2) = nresponse;
    SET_SP(vm, sp, 1);
    return ip;
}


/* ( c-addr u1 fileid -- u2 flag ior ) */
PRIM_HDLR(x_read_line)
{
    cell_ft *sp = SP(vm);
    FILE *fp;
    cell_ft nrequest;
    char *buff;

    CHECK_POP(vm, 3);

    fp = (FILE *) PICK(sp, 0);
    nrequest = PICK(sp, 1);
    buff = (char *) PICK(sp, 2);

    if (nrequest > INT_MAX) {
	PICK(sp, 2) = 0;
	PICK(sp, 1) = F_FALSE;
	PICK(sp, 0) = -1;
    } else if (fgets(buff, (int) nrequest, fp) != NULL) {
	size_t len = strlen(buff) - 1;
	if (buff[len] != '\n') {
	    len++;
	    if (!feof(fp)) {
		int c = getc(fp);
		if (c != EOF) {
		    buff[len] = c;
		    if (c != '\n') {
			len++;
		    }
		}
	    }
	}
	PICK(sp, 2) = len;
	PICK(sp, 1) = F_TRUE;
	PICK(sp, 0) = 0;
    } else {
	PICK(sp, 2) = 0;
	PICK(sp, 1) = F_FALSE;
	if (feof(fp)) {
	    PICK(sp, 0) = 0;
	} else if (ferror(fp)) {
	    PICK(sp, 0) = errno;
	    clearerr(fp);
	} else {
	    PICK(sp, 0) = UNKNOWN_ERROR;
	}
    }
    return ip;
}


/* ( ud fileid -- ior ) */
PRIM_HDLR(x_reposition_file)
{
    cell_ft *sp = SP(vm);
    FILE *fp;
    long fpos;

    CHECK_POP(vm, 3);
    fp = (FILE *) PICK(sp, 0);
    fpos = (long) PICK(sp, 2);
    // FIXME:  REPOSITION-FILE when sizeof (ud) != sizeof (long)
    if (PICK(sp, 1) == 0) {
	if (fseek(fp, fpos, SEEK_SET) >= 0) {
	    PICK(sp, 2) = 0;
	} else {
	    PICK(sp, 2) = errno;
	}
    } else {
	PICK(sp, 2) = (cell_ft) EOVERFLOW;
    }
    SET_SP(vm, sp, 2);
    return ip;
}


/* ( c-addr u fileid -- ior ) */
PRIM_HDLR(x_write_file)
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
    } else if (ferror(fp)) {
	PICK(sp, 2) = errno;
	clearerr(fp);
    } else {
	PICK(sp, 2) = UNKNOWN_ERROR;
    }
    SET_SP(vm, sp, 2);
    return ip;
}


/* ( c-addr u fileid -- ior ) */
PRIM_HDLR(x_write_line)
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
    if (nresponse == nrequest && putc('\n', fp) != EOF) {
	PICK(sp, 2) = 0;
    } else if (ferror(fp)) {
	PICK(sp, 2) = errno;
	clearerr(fp);
    } else {
	PICK(sp, 2) = UNKNOWN_ERROR;
    }
    SET_SP(vm, sp, 2);
    return ip;
}
