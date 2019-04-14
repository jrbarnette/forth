# Copyright 2010, by J. Richard Barnette

SOURCES =			\
	    arithops.c		\
	    cmdline.c		\
	    control.c		\
	    dictionary.c	\
	    direct.c		\
	    exception.c		\
	    fileops.c		\
	    forth.c		\
	    indirect.c		\
	    initdict.c		\
	    memops.c		\
	    meta.c		\
	    multops.c		\
	    names.c		\
	    parse.c		\
	    stackops.c		\
	    terminal.c		\
	    vm.c

CMDLINE_DEP = forth.o parse.o exception.o cmdline.o
DICT_DEP =  dictionary.o	\
	    exception.o		\
	    forth.o		\
	    initdict.o		\
	    names.o		\
	    parse.o		\
	    vm.o

PRIM_DEP =  $(DICT_DEP)         \
            arithops.o		\
	    control.o		\
	    direct.o		\
	    fileops.o		\
	    indirect.o		\
	    memops.o		\
	    multops.o		\
	    stackops.o		\
	    terminal.o

CHECKNONE = 0
CHECKSTMT = 1
CHECKEXPR = 2
CHECKMODE = EXPR
CHECKFLAG = -DCHECKMODE=$(CHECK$(CHECKMODE))

OBJECTS = $(SOURCES:.c=.o)

CFLAGS = -O -g $(CHECKFLAG) -Wall
LDFLAGS = -lreadline

all: forth tags

$(CMDLINE_DEP): cmdline.h
$(DICT_DEP): dictionary.h
$(PRIM_DEP): forth.h
initdict.o: direct.h

forth: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

$(OBJECTS): forth.h

tags: $(SOURCES)
	ctags $(SOURCES)

clean:
	rm -f forth tags $(OBJECTS) stdforth.c
