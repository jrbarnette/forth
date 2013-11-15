# Copyright 2013, by J. Richard Barnette. All Rights Reserved.

SOURCES =			\
		arithops.c	\
		compile.c	\
		control.c	\
		dictionary.c	\
		format.c	\
		forth.c		\
		interpret.c	\
		memops.c	\
		multops.c	\
		names.c		\
		parse.c		\
		stackops.c	\
		terminal.c	\
		test.c		\
		vm.c

CHECKNONE = 0
CHECKSTMT = 1
CHECKEXPR = 2
CHECKMODE = EXPR
CHECKFLAG = -DCHECKMODE=$(CHECK$(CHECKMODE))
OPTIONS = $(CHECKFLAG) -DTEST

OBJECTS = $(SOURCES:.c=.o)
CFLAGS = -O -g $(OPTIONS) -Wall

all: forth tags

forth: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

$(OBJECTS): forth.h

tags: $(SOURCES)
	ctags $(SOURCES)

clean:
	rm -f forth tags $(OBJECTS)
