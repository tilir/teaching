# try: make CC=clang CFLAGS="-g -O0"

CC ?= gcc
CFLAGS ?= -O2
COMMONINC = -I./include
TESTS = ./Tests

CFLAGS += $(COMMONINC)

all: LCmain.o cache.o hash.o list.o
	$(CC) $(LDFLAGS) LCmain.o cache.o hash.o list.o -o LCsep.x

LCmain.o: LCmain.c
	$(CC) $(CFLAGS) -c $< -o $@

cache.o: ./source/cache.c
	$(CC) $(CFLAGS) -c $< -o $@

hash.o: ./source/hash.c
	$(CC) $(CFLAGS) -c $< -o $@

list.o: ./source/list.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: testsep
testsep: LCsep.x
	@for i in $(TESTS)/*.dat; do echo "$$(basename $${i})"; ./LCsep.x < $${i}; echo ""; done > all.log
	@diff -q -w all.log $(TESTS)/corr.log || echo "Tests failed"

.PHONY: clean
clean:
	rm *.x
	rm -rf *.o
