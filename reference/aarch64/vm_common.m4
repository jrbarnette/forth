dnl  Copyright 2025, by J. Richard Barnette. All Rights Reserved.
dnl
dnl
include(`c_defs.m4')
dnl
dnl These are private to the assembly implementation, but are meant to
dnl be architecture independent.
dnl
define(`CELLS', `* 'eval(1 << CELL_SHIFT))dnl
define(`CELL_SIZE', eval(1 CELLS))dnl
define(`CELL_BITS', eval(8 * (1 CELLS)))dnl
define(`SENTINEL32', `0xdeadbeef')dnl
define(`SENTINEL64', `(0xfeedface<<32)+SENTINEL32')dnl
define(`SENTINEL', `SENTINEL'CELL_BITS)dnl
dnl
define(`XFRAME_VXSP',  `0')dnl
define(`XFRAME_VSP',   eval(1 CELLS))dnl
define(`XFRAME_VIP',   eval(2 CELLS))dnl
define(`XFRAME_SIZE',  eval(3 CELLS))dnl
