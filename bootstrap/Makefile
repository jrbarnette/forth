# Copyright 2010, by J. Richard Barnette

COMMON_SRC =			\
	    arithops.c		\
	    cmdline.c		\
	    control.c		\
	    definers.c		\
	    direct.c		\
	    exception.c		\
	    fileops.c		\
	    memops.c		\
	    multops.c		\
	    stackops.c		\
	    terminal.c		\
	    vm.c

FORTH_SRC =			\
	    allot.c		\
	    forth.c		\
	    initdict.c		\
	    meta.c		\
	    names.c

FORTH_OBJ =  $(COMMON_SRC:.c=.o) $(FORTH_SRC:.c=.o)
DIRECT_OBJ = $(COMMON_SRC:.c=.o) $(DIRECT_SRC:.c=.o)
HEADER_SRC =	cmdline.h	dictionary.h	forth.h		\
		ftypes.h	meta.h		prim.h		\
		vmcheck.h	vmstate.h	xtoken.h

SOURCES = $(COMMON_SRC) $(FORTH_SRC) $(DIRECT_SRC) $(HEADER_SRC)

# everything depending on cmdline.h
CMDLINE_DEP = cmdline.o exception.o forth.o terminal.o

# everything depending only on forth.h and its constituents
PRIM_DEP =      arithops.o	control.o	definers.o	\
		direct.o	fileops.o	memops.o	\
		multops.o	stackops.o	vm.o

# everything depending on dictionary.h but not meta.h
DICT_DEP =	exception.o	forth.o		terminal.o

# everything depending on meta.h but not dictionary.h
META_DEP =	meta.o

# everything depending on dictionary.h *and* meta.h
DIRECT_DEP =	allot.o		initdict.o	names.o


PRIM_HDRS = forth.h ftypes.h vmstate.h xtoken.h vmcheck.h prim.h
DICT_HDRS = dictionary.h
META_HDRS = meta.h

CHECKENABLE = CHECK
CHECKDISABLE = NOCHECK
CHECKMODE = ENABLE
CHECKFLAG = -DSTACKCHECK=$(CHECK$(CHECKMODE))
OTHER_FLAGS =

# By default, -O doesn't apply the optimizations needed to open-code
# functions declared with `inline`.  -O2 does include that optimization.
# This isn't exactly tested on Linux/gcc, but online documentation seems
# to indicate they're the same, or close enough.
OPT_FLAG = -O2 -g
CFLAGS = $(OPT_FLAG) $(CHECKFLAG) -Wall $(OTHER_FLAGS)
LDFLAGS = -lreadline

all: forth tags

$(CMDLINE_DEP): cmdline.h
$(PRIM_DEP): $(PRIM_HDRS)
$(DICT_DEP): $(PRIM_HDRS) $(DICT_HDRS)
$(META_DEP): $(PRIM_HDRS) $(META_HDRS)
$(DIRECT_DEP): $(PRIM_HDRS) $(DICT_HDRS) $(META_HDRS)

forth: $(FORTH_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(FORTH_OBJ)

tags: $(SOURCES)
	ctags -w $(SOURCES)

clean:
	rm -f forth tags $(FORTH_OBJ) $(DIRECT_OBJ) stdforth.c
