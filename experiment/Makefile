CFLAGS = -O2

TARGETS = tos ip thr cdivbench

BENCH_SOURCES = bench.c compile.c
CDIV_SOURCES = cdivbench.c cdivmod.c
TOS_SOURCES = $(BENCH_SOURCES) tos.c
IP_SOURCES = $(BENCH_SOURCES) ip.c

TOS_OBJECTS = $(TOS_SOURCES:.c=.o)
IP_OBJECTS = $(IP_SOURCES:.c=.o)
THR_OBJECTS = $(BENCH_SOURCES:.c=.o) thr.o
BENCH_OBJECTS = $(BENCH_SOURCES:.c=.o) ip.o tos.o
CDIV_OBJECTS = $(CDIV_SOURCES:.c=.o)
OBJECTS = $(BENCH_OBJECTS) thr.o $(CDIV_OBJECTS)

all: $(TARGETS)

tos: $(TOS_OBJECTS)
	$(CC) -o $@ $(CFLAGS) $^

ip: $(IP_OBJECTS)
	$(CC) -o $@ $(CFLAGS) $^

thr: $(THR_OBJECTS)
	$(CC) -o $@ $(CFLAGS) $^

cdivbench: $(CDIV_OBJECTS)
	$(CC) -o $@ $(CFLAGS) $^

thr.s: thr_arm64.m4
	m4 thr_arm64.m4 > $@

$(BENCH_OBJECTS): bench.h

clean:
	rm -f $(TARGETS) $(OBJECTS)
