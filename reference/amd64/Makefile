SOURCES =	arithops.m4	control.m4	definers.m4	fileops.m4 \
		memops.m4	multops.m4	stackops.m4	terminal.m4 \
		vm.m4

ASM_FILES = $(SOURCES:.m4=.s)

OBJECTS = $(SOURCES:.m4=.o)

%.s: %.m4
	m4 $< > $@

all: $(OBJECTS)

$(ASM_FILES): prim.m4

clean:
	rm -f $(ASM_FILES) $(OBJECTS)
