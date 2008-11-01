SOURCES =			\
	compile.c		\
	dictionary.c		\
	forth.c			\
	primitives.c		\
	termio.c		\
	vm.c

OBJECTS = $(SOURCES:.c=.o)

all: forth tags

forth: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

tags: $(SOURCES)
	ctags $(SOURCES)
