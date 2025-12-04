/*
 * Copyright 2025, by J. Richard Barnette. All Rights Reserved.
 */

#ifndef FILEIO_H
#define FILEIO_H

#include "forth.h"

#define FILE_MODE_R_O		((cell_ft) 001)
#define FILE_MODE_R_W		((cell_ft) 002)
#define FILE_MODE_W_O		((cell_ft) 003)
#define FILE_MODE_BIN		((cell_ft) 004)
#define FILE_MODE_CREATE	((cell_ft) 010)

extern cell_ft file_open(FILE **fpp, cell_ft fam, c_addr_ft str,
			 cell_ft len);
extern cell_ft file_close(FILE *fp);
extern cell_ft file_position(FILE *fp, dcell_ft *d);
extern cell_ft file_reposition_file(FILE *fp, cell_ft hi, long fpos);
extern cell_ft file_size(FILE *fp, dcell_ft *d);
extern cell_ft file_read(FILE *fp, c_addr_ft buff, cell_ft nrequest,
			 cell_ft *nresponse);
extern cell_ft file_read_line(FILE *fp, c_addr_ft buff,
			      cell_ft nrequest, cell_ft *nresponse,
			      cell_ft *flag);
extern cell_ft file_write_file(FILE *fp, c_addr_ft buff,
			       cell_ft nrequest);
extern cell_ft file_write_line(FILE *fp, c_addr_ft buff,
			       cell_ft nrequest);

#endif // FILEIO_H
