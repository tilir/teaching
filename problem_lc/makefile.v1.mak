#------------------------------------------------------------------------------
#
# Makefile, approach 1
# * automatic variables expose the redundancy of boilerplate code 
# * override concept illustrated
# * CC "redefined" but actually not
#
#------------------------------------------------------------------------------
#
# This file is licensed after LGPL v3
# Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
#
#------------------------------------------------------------------------------

# make CC=clang CFLAGS="-g -O0" -f makefile.v1.mak
# make -f makefile.v1.mak clean
# make -f makefile.v1.mak
# make -f makefile.v1.mak testrun
# make -f makefile.v1.mak clean

CC ?= gcc
CFLAGS ?= -O2
COMMONINC = -I./include
TESTS = ./Tests

override CFLAGS += $(COMMONINC)

.PHONY: all
all: LC.x

LC.x: LCmain.o cache.o hash.o list.o
	$(CC) $^ -o $@ $(LDFLAGS)

LCmain.o: LCmain.c
	$(CC) $(CFLAGS) -c $^ -o $@

cache.o: ./source/cache.c
	$(CC) $(CFLAGS) -c $^ -o $@

hash.o: ./source/hash.c
	$(CC) $(CFLAGS) -c $^ -o $@

list.o: ./source/list.c
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: testrun
testrun: LC.x
	@for i in $(TESTS)/*.dat; do echo "$$(basename $${i})"; ./LC.x < $${i}; echo ""; done > all.log
	@if diff -w all.log $(TESTS)/corr.log; then echo "Tests pass"; else echo "Tests failed"; fi

.PHONY: clean
clean:
	rm *.x
	rm -rf *.o
	rm -rf *.log
