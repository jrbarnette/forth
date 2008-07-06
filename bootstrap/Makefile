SOURCES =			\
	compile.c		\
	dictionary.c		\
	init.c			\
	primitives.c		\
	termio.c		\
	vm.c

OBJECTS = $(SOURCES:.c=.o)

forth: forth.o $(OBJECTS)
	$(CC) -o $@  forth.o $(OBJECTS)

test: test.o $(OBJECTS)
	$(CC) -o $@  forth.o $(OBJECTS)
