H_FILES =	cforth.h ftypes.h prim.h
SOURCES =	arithops.m4	control.m4	execute.m4	fileops.m4 \
		memops.m4	multops.m4	stackops.m4	terminal.m4

ASM_FILES = $(SOURCES:.m4=.s)

OBJECTS = $(SOURCES:.m4=.o) primtest.o

CFLAGS = -O

%.s: %.m4
	m4 $< > $@

all: primtest

primtest: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(ASM_FILES): prim.m4

clean:
	rm -f $(ASM_FILES) $(OBJECTS) primtest
