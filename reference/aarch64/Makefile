H_FILES =	cforth.h ftypes.h prim.h
M4_INCLUDES =	vm.m4 vm_common.m4 c_defs.m4
SOURCES =	arithops.m4	execute.m4	stackops.m4
# SOURCES =	control.m4	fileops.m4 \
# 		memops.m4	multops.m4	terminal.m4

ASM_FILES = $(SOURCES:.m4=.s)

OBJECTS = $(SOURCES:.m4=.o) primtest.o

CFLAGS = -O

all: primtest

%.s: %.m4
	m4 $< > $@

primtest: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

primtest.o: primtest.c $(H_FILES)

$(ASM_FILES): $(M4_INCLUDES)

cforth.h: cforth.m4 c_defs.m4
	m4 cforth.m4 > $@

clean:
	rm -f $(ASM_FILES) $(OBJECTS) primtest cforth.h
