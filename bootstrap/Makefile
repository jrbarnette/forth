# Copyright 2010, by J. Richard Barnette

SOURCES =			\
	    arithops.c		\
	    cmdline.c		\
	    control.c		\
	    dictionary.c	\
	    fileops.c		\
	    format.c		\
	    forth.c		\
	    interpret.c		\
	    memops.c		\
	    multops.c		\
	    names.c		\
	    stackops.c		\
	    stdforth.c		\
	    termio.c

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

stdforth.c: gen_cforth stdforth.fth
	./gen_cforth initialize_forth stdforth.fth > $@

tags: $(SOURCES)
	ctags $(SOURCES)

clean:
	rm -f forth tags $(OBJECTS)
