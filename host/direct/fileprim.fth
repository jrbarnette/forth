\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\  fileprim.fth - Standard Forth words for the optional File-Access
\    word set.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  (                  11.6.1.0080 FILE                   80
\  BIN                11.6.1.0765 FILE                   80
\  CLOSE-FILE         11.6.1.0900 FILE                   80
\  CREATE-FILE        11.6.1.1010 FILE                   81
\  DELETE-FILE        11.6.1.1190 FILE                   81
\  FILE-POSITION      11.6.1.1520 FILE                   81
\  FILE-SIZE          11.6.1.1522 FILE                   81
\  INCLUDE-FILE       11.6.1.1717 FILE                   81
\  INCLUDED           11.6.1.1718 FILE                   82
\  OPEN-FILE          11.6.1.1970 FILE                   82
\  R/O                11.6.1.2054 FILE                   82
\  R/W                11.6.1.2056 FILE                   83
\  READ-FILE          11.6.1.2080 FILE                   83
\  READ-LINE          11.6.1.2090 FILE                   83
\  REPOSITION-FILE    11.6.1.2142 FILE                   84
\  RESIZE-FILE        11.6.1.2147 FILE                   84
\  S"                 11.6.1.2165 FILE                   84
\  SOURCE-ID          11.6.1.2218 FILE                   84
\  W/O                11.6.1.2425 FILE                   85
\  WRITE-FILE         11.6.1.2480 FILE                   85
\  WRITE-LINE         11.6.1.2485 FILE                   85
\
\  FILE-STATUS        11.6.2.1524 FILE EXT               85
\  FLUSH-FILE         11.6.2.1560 FILE EXT               85
\  REFILL             11.6.2.2125 FILE EXT               86
\  RENAME-FILE        11.6.2.2130 FILE EXT               86
\ ------  ------  ------  ------  ------  ------  ------  ------

\ FILE
\     (
prim: BIN               x_bin
prim: CLOSE-FILE        x_close_file
prim: CREATE-FILE       x_create_file
\     DELETE-FILE
prim: FILE-POSITION     x_file_position
prim: FILE-SIZE         x_file_size
\     INCLUDE-FILE
\     INCLUDED
prim: OPEN-FILE         x_open_file
prim: R/O               x_r_o
prim: R/W               x_r_w
prim: READ-FILE         x_read_file
\     READ-LINE
prim: REPOSITION-FILE   x_reposition_file
\     RESIZE-FILE
\     S"
\     SOURCE-ID
prim: W/O               x_w_o
prim: WRITE-FILE        x_write_file
\     WRITE-LINE

\ FILE EXT
\     FILE-STATUS
\     FLUSH-FILE
\     REFILL
\     RENAME-FILE
