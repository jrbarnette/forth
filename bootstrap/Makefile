# Copyright 2010, by J. Richard Barnette

PRIM_SOURCES =			\
	    arithops.c		\
	    control.c		\
	    definers.c		\
	    memops.c		\
	    multops.c		\
	    stackops.c		\
	    vm.c

VM_SOURCES = 			\
	    cmdline.c		\
	    fileio.c		\
	    fileops.c		\
	    initdict.c		\
	    terminal.c		\
	    termops.c

TEST_SOURCES = primtest.c $(PRIM_SOURCES)
FORTH_SOURCES =	forth.c $(PRIM_SOURCES) $(VM_SOURCES)

TEST_OBJECTS =  $(TEST_SOURCES:.c=.o)
FORTH_OBJECTS =  $(FORTH_SOURCES:.c=.o)

C_SOURCES = primtest.c forth.c $(VM_SOURCES) $(PRIM_SOURCES)
OBJECTS = $(C_SOURCES:.c=.o)

H_SOURCES =	cforth.h	cmdline.h	fileio.h	\
		forth.h		ftypes.h	prim.h		\
		terminal.h	vmcheck.h	vmstate.h	\
		xtoken.h

SOURCES = $(C_SOURCES) $(H_SOURCES)

# everything depending on cmdline.h
CMDLINE_DEP = cmdline.o forth.o

# everything depending on fileio.h
FILEIO_DEP = fileio.o fileops.o initdict.o

# everything depending on cforth.h
CFORTH_DEP = forth.o initdict.o vm.o

# everything depending on terminal.h
TERMINAL_DEP = forth.o terminal.o termops.o

# everything depending on forth.h and its constituents
PRIM_DEP =	arithops.c	control.c	definers.c	\
		fileio.c	fileops.c	forth.c		\
		initdict.c	memops.c	multops.c	\
		stackops.c	terminal.c	termops.c	\
		vm.c

PRIM_HDRS = forth.h ftypes.h vmstate.h xtoken.h vmcheck.h prim.h

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

all: primtest forth tags

$(CMDLINE_DEP): cmdline.h
$(FILEIO_DEP): $(PRIM_HDRS) fileio.h
$(CFORTH_DEP): ftypes.h vmstate.h xtoken.h cforth.h
$(TERMINAL_DEP): terminal.h
$(PRIM_DEP): $(PRIM_HDRS)

primtest: $(TEST_OBJECTS) $(CFORTH_DEP)
	$(CC) -o $@ $(TEST_OBJECTS) $(LDFLAGS)
	./$@

forth: $(FORTH_OBJECTS)
	$(CC) -o $@ $(FORTH_OBJECTS) $(LDFLAGS)

tags: $(SOURCES)
	ctags -w $(SOURCES)

clean:
	rm -f primtest forth tags $(OBJECTS)
