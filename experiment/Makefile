CFLAGS = -O

TARGETS = tos ip cdivbench

all: $(TARGETS)

clean:
	rm -f $(TARGETS)

tos: bench.h bench.c tos.c
	$(CC) -o $@ $(CFLAGS) bench.c tos.c

ip: bench.h bench.c ip.c
	$(CC) -o $@ $(CFLAGS) bench.c ip.c

cdivbench: cdivbench.o cdivmod.o
	$(CC) -o $@ $(CFLAGS) $^
