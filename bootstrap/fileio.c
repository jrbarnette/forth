/*
 * Copyright 2025, by J. Richard Barnette. All Rights Reserved.
 */

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "forth.h"
#include "fileio.h"


#define UNKNOWN_ERROR   ((cell_ft) -1)

static cell_ft
ferror_to_ior(FILE *fp)
{
    if (ferror(fp)) {
	clearerr(fp);
	return (cell_ft) errno;
    } else {
	return UNKNOWN_ERROR;
    }
}


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
 *
 */
cell_ft
file_open(FILE **fpp, cell_ft fam, c_addr_ft str, cell_ft len)
{
    char filename[256];
    cell_ft ior;

    assert(sizeof (*str) == sizeof (*filename));

    if (len < sizeof (filename)) {
	cell_ft mode_flag;
	char fmode[4], *cp = fmode;

	memcpy(filename, str, len);
	filename[len] = '\0';

	if ((fam & FILE_MODE_CREATE) == 0) {
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
	*fpp = fopen(filename, fmode);
	if (*fpp == NULL) {
	    ior = errno;
	} else {
	    ior = 0;
	}
    } else {
	*fpp = NULL;
	ior = ENAMETOOLONG;
    }

    return ior;
}


cell_ft
file_close(FILE *fp)
{
    if (fclose(fp) == 0) {
	return 0;
    } else {
	return (cell_ft) errno;
    }
}


cell_ft
file_position(FILE *fp, dcell_ft *d)
{
    long fpos = ftell(fp);
    // FIXME: FILE-POSITION restricted to single cell result
    d->hi = 0;
    d->lo = (cell_ft) fpos;
    if (fpos >= 0) {
	return 0;
    } else {
	return errno;
    }
}


cell_ft
file_reposition_file(FILE *fp, cell_ft hi, long fpos)
{
    // FIXME:  REPOSITION-FILE when ud exceeds LONG_MAX
    if (hi == 0 && fpos >= 0) {
	if (fseek(fp, fpos, SEEK_SET) >= 0) {
	    return 0;
	} else {
	    return (cell_ft) errno;
	}
    } else {
	return (cell_ft) EOVERFLOW;
    }
}


cell_ft
file_size(FILE *fp, dcell_ft *d)
{
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

    // FIXME: FILE-SIZE restricted to single cell result
    d->hi = (cell_ft) 0;
    d->lo = (cell_ft) fsize;

    return ior;
}


cell_ft
file_read(FILE *fp, c_addr_ft buff, cell_ft nrequest, cell_ft *nresponse)
{
    *nresponse = fread(buff, sizeof (*buff), (size_t) nrequest, fp);
    if (*nresponse == nrequest) {
	return 0;
    } else if (feof(fp)) {
	return 0;
    } else {
	return ferror_to_ior(fp);
    }
}


cell_ft
file_read_line(FILE *fp, c_addr_ft buff, cell_ft nrequest,
	       cell_ft *nresponse, cell_ft *flag)
{
    if (nrequest > INT_MAX) {
	*nresponse = 0;
	*flag = F_FALSE;
	return UNKNOWN_ERROR;
    } else if (fgets((char *) buff, (int) nrequest, fp) != NULL) {
	size_t len = strlen((char *) buff) - 1;
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
	*nresponse = len;
	*flag = F_TRUE;
	return 0;
    } else {
	*nresponse = 0;
	*flag = F_FALSE;
	if (feof(fp)) {
	    return 0;
	} else {
	    return ferror_to_ior(fp);
	}
    }
}


cell_ft
file_write_file(FILE *fp, c_addr_ft buff, cell_ft nrequest)
{
    size_t nresponse = fwrite(buff, sizeof (*buff),
			      (size_t) nrequest, fp);
    if (nresponse == nrequest) {
	return 0;
    } else {
	return ferror_to_ior(fp);
    }
}


cell_ft
file_write_line(FILE *fp, c_addr_ft buff, cell_ft nrequest)
{
    size_t nresponse = fwrite(buff, sizeof (*buff),
			      (size_t) nrequest, fp);
    if (nresponse == nrequest && putc('\n', fp) != EOF) {
	return 0;
    } else {
	return ferror_to_ior(fp);
    }
}
