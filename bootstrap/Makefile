SOURCES =			\
	compile.c		\
	dictionary.c		\
	format.c		\
	forth.c			\
	interpret.c		\
	primitives.c		\
	termio.c

OBJECTS = $(SOURCES:.c=.o)
CFLAGS = -O -g

all: forth tags

forth: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

$(OBJECTS): forth.h

tags: $(SOURCES)
	ctags $(SOURCES)

clean:
	rm -f forth tags $(OBJECTS)
