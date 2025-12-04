# Copyright 2010, by J. Richard Barnette

C_SOURCES =			\
	    arithops.c		\
	    cmdline.c		\
	    control.c		\
	    definers.c		\
	    exception.c		\
	    fileio.c		\
	    fileops.c		\
	    forth.c		\
	    initdict.c		\
	    memops.c		\
	    multops.c		\
	    stackops.c		\
	    terminal.c		\
	    vm.c

OBJECTS =  $(C_SOURCES:.c=.o)
H_SOURCES =	cmdline.h	dictionary.h	forth.h		\
		ftypes.h	prim.h		vmcheck.h	\
		vmstate.h	xtoken.h

SOURCES = $(C_SOURCES) $(H_SOURCES)

# everything depending on cmdline.h
CMDLINE_DEP = cmdline.o exception.o forth.o terminal.o

# everything depending on fileio.h
FILEIO_DEP = fileio.o fileops.o initdict.o

# everything depending only on forth.h and its constituents
PRIM_DEP =      arithops.o	control.o	definers.o	\
		fileops.o	memops.o	multops.o	\
		stackops.o	vm.o

# everything depending on dictionary.h
DICT_DEP =	exception.o	forth.o		initdict.o	\
		terminal.o

PRIM_HDRS = forth.h ftypes.h vmstate.h xtoken.h vmcheck.h prim.h cforth.h

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
$(FILEIO_DEP): fileio.h
$(PRIM_DEP): $(PRIM_HDRS)
$(DICT_DEP): $(PRIM_HDRS) dictionary.h

forth: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

tags: $(SOURCES)
	ctags -w $(SOURCES)

clean:
	rm -f forth tags $(OBJECTS)
