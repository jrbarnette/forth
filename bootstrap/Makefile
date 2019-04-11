# Copyright 2010, by J. Richard Barnette

SOURCES =			\
	    arithops.c		\
	    cmdline.c		\
	    control.c		\
	    dictionary.c	\
	    fileops.c		\
	    forth.c		\
	    initdict.c		\
	    memops.c		\
	    multops.c		\
	    names.c		\
	    parse.c		\
	    stackops.c		\
	    terminal.c		\
	    vm.c

CHECKNONE = 0
CHECKSTMT = 1
CHECKEXPR = 2
CHECKMODE = EXPR
CHECKFLAG = -DCHECKMODE=$(CHECK$(CHECKMODE))

OBJECTS = $(SOURCES:.c=.o)
CFLAGS = -O -g $(CHECKFLAG) -Wall
LDFLAGS = -lreadline

all: forth tags

forth: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

$(OBJECTS): forth.h

tags: $(SOURCES)
	ctags $(SOURCES)

clean:
	rm -f forth tags $(OBJECTS) stdforth.c
