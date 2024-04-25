#------------------------------------------------------------------------------
#
# Makefile, approach 3
# * implicit rule for first dependency not for all
# * dependency gather filtered
# * pattern substitution
#
#------------------------------------------------------------------------------
#
# This file is licensed after LGPL v3
# Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
#
#------------------------------------------------------------------------------

# make CC=clang CFLAGS="-g -O0" -f makefile.v3.mak
# make -f makefile.v3.mak clean
# make -f makefile.v3.mak
# touch include/cache.h
# make -f makefile.v3.mak
# make -f makefile.v3.mak testrun
# make -f makefile.v3.mak clean

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
DEPS = $(COBJ:.o=.d)

.PHONY: all
all: LC.x

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
-include $(DEPS)
endif

LC.x: $(COBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o : source/%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.d : source/%.c
	$(CC) -E $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@

%.d : %.c
	$(CC) -E $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@

.PHONY: testrun
testrun: LC.x
	./runtests.sh $(TESTS)

.PHONY: clean
clean:
	rm -rf *.x
	rm -rf *.o
	rm -rf *.log
	rm -rf *.d
