CFLAGS = -O -m32

all: tos ip

tos: bench.h bench.c tos.c
	$(CC) -o $@ $(CFLAGS) bench.c tos.c

ip: bench.h bench.c ip.c
	$(CC) -o $@ $(CFLAGS) bench.c ip.c
