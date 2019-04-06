\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  (                  11.6.1.0080 FILE
\  BIN                11.6.1.0765 FILE
\  CLOSE-FILE         11.6.1.0900 FILE
\  CREATE-FILE        11.6.1.1010 FILE
\  DELETE-FILE        11.6.1.1190 FILE
\  FILE-POSITION      11.6.1.1520 FILE
\  FILE-SIZE          11.6.1.1522 FILE
\  INCLUDE-FILE       11.6.1.1717 FILE
\  INCLUDED           11.6.1.1718 FILE
\  OPEN-FILE          11.6.1.1970 FILE
\  R/O                11.6.1.2054 FILE
\  R/W                11.6.1.2056 FILE
\  READ-FILE          11.6.1.2080 FILE
\  READ-LINE          11.6.1.2090 FILE
\  REPOSITION-FILE    11.6.1.2142 FILE
\  RESIZE-FILE        11.6.1.2147 FILE
\  S"                 11.6.1.2165 FILE
\  SOURCE-ID          11.6.1.2218 FILE
\  W/O                11.6.1.2425 FILE
\  WRITE-FILE         11.6.1.2480 FILE
\  WRITE-LINE         11.6.1.2485 FILE
\
\  FILE-STATUS        11.6.2.1524 FILE EXT
\  FLUSH-FILE         11.6.2.1560 FILE EXT
\  REFILL             11.6.2.2125 FILE EXT
\  RENAME-FILE        11.6.2.2130 FILE EXT
\ ------  ------  ------  ------  ------  ------  ------  ------

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
prim: READ-LINE         x_read_line
prim: REPOSITION-FILE   x_reposition_file
\     RESIZE-FILE
\     S"
\     SOURCE-ID
prim: W/O               x_w_o
prim: WRITE-FILE        x_write_file
prim: WRITE-LINE        x_write_line

\     FILE-STATUS
\     FLUSH-FILE
\     REFILL
\     RENAME-FILE
