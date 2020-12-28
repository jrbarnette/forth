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

DIRECT_SRC = direct_test.c

FORTH_OBJ =   $(COMMON_SRC:.c=.o) $(FORTH_SRC:.c=.o)
DIRECT_OBJ =  $(COMMON_SRC:.c=.o) $(DIRECT_SRC:.c=.o)
HEADER_SRC =	cmdline.h	dictionary.h	direct.h	\
		forth.h		ftypes.h	names.h		\
		prim.h		vmcheck.h	vmstate.h	\
	    	xtoken.h

SOURCES = $(COMMON_SRC) $(FORTH_SRC) $(DIRECT_SRC) $(HEADER_SRC)

CMDLINE_DEP = forth.o exception.o cmdline.o direct_test.o terminal.o

DIRECT_DEP =	allot.o		initdict.o	meta.o		names.o
DICT_DEP =	exception.o	forth.o		terminal.o
PRIM_DEP =	control.o	definers.o	direct.o	\
		direct_test.o	fileops.o	memops.o	\
		multops.o	stackops.o	arithops.o	\
		vm.o

PRIM_HDRS = forth.h ftypes.h vmstate.h xtoken.h vmcheck.h prim.h
DICT_HDRS = names.h dictionary.h
DIRECT_HDRS = direct.h

CHECKENABLE = CHECK
CHECKDISABLE = NOCHECK
CHECKMODE = ENABLE
CHECKFLAG = -DSTACKCHECK=$(CHECK$(CHECKMODE))

CFLAGS = -O -g $(CHECKFLAG) -Wall
LDFLAGS = -lreadline

all: forth tags

$(CMDLINE_DEP): cmdline.h
$(PRIM_DEP): $(PRIM_HDRS)
$(DICT_DEP): $(PRIM_HDRS) $(DICT_HDRS)
$(DIRECT_DEP): $(PRIM_HDRS) $(DICT_HDRS) $(DIRECT_HDRS)
direct_test.o: $(PRIM_HDRS) names.h direct.h

forth: $(FORTH_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(FORTH_OBJ)

direct_test: $(DIRECT_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(DIRECT_OBJ)

tags: $(SOURCES)
	ctags -w $(SOURCES)

clean:
	rm -f forth tags $(FORTH_OBJ) $(DIRECT_OBJ) stdforth.c
