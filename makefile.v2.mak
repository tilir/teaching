#------------------------------------------------------------------------------
#
# Makefile, approach 2
#
#------------------------------------------------------------------------------
#
# This file is licensed after LGPL v3
# Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
#
#------------------------------------------------------------------------------

# make CC=clang CFLAGS="-g -O0" -f makefile.v2.mak
# make -f makefile.v2.mak clean
# make -f makefile.v2.mak
# make -f makefile.v2.mak testrun
# make -f makefile.v2.mak clean

ifeq ($(origin CC),default)
  CC = gcc
endif

CFLAGS ?= -O2
COMMONINC = -I./include
TESTS = ./Tests
SRC = ./source

override CFLAGS += $(COMMONINC)

CSRC = LCmain.c source/cache.c source/hash.c source/list.c
COBJ = LCmain.o cache.o hash.o list.o

%.o : source/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

LC.x: $(COBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

.PHONY: testrun
testrun: LC.x
	@for i in $(TESTS)/*.dat; do echo "$$(basename $${i})"; ./LC.x < $${i}; echo ""; done > all.log
	@if diff -w all.log $(TESTS)/corr.log; then echo "Tests pass"; else echo "Tests failed"; fi

.PHONY: clean
clean:
	rm -rf *.x
	rm -rf *.o
	rm -rf *.log
