# Copyright 2013, by J. Richard Barnette. All Rights Reserved.

SOURCES =			\
		arithops.c	\
		compile.c	\
		control.c	\
		dictionary.c	\
		format.c	\
		forth.c		\
		memops.c	\
		multops.c	\
		names.c		\
		stackops.c	\
		terminal.c	\
		vm.c

CHECKNONE = 0
CHECKSTMT = 1
CHECKEXPR = 2
CHECKMODE = EXPR
CHECKFLAG = -DCHECKMODE=$(CHECK$(CHECKMODE))

OBJECTS = $(SOURCES:.c=.o)
CFLAGS = -O -g $(CHECKFLAG) -Wall

all: forth tags

forth: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

$(OBJECTS): forth.h

tags: $(SOURCES)
	ctags $(SOURCES)

clean:
	rm -f forth tags $(OBJECTS)
