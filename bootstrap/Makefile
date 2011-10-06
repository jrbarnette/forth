# Copyright 2010, by J. Richard Barnette

SOURCES =			\
	    arithops.c		\
	    control.c		\
	    dictionary.c	\
	    format.c		\
	    forth.c		\
	    interpret.c		\
	    memops.c		\
	    multops.c		\
	    names.c		\
	    stackops.c		\
	    termio.c

CHECKNONE = 0
CHECKSTMT = 1
CHECKEXPR = 2
CHECKMODE = EXPR
CHECKFLAG = -DCHECKMODE=$(CHECK$(CHECKMODE))

OBJECTS = $(SOURCES:.c=.o)
CFLAGS = -O -g $(CHECKFLAG)

all: forth tags

forth: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

$(OBJECTS): forth.h

tags: $(SOURCES)
	ctags $(SOURCES)

clean:
	rm -f forth tags $(OBJECTS)
