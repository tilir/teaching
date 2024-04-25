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

# make OUT_O_DIR=debug CC=clang CFLAGS="-g -O0" -f makefile.v3.mak
# make -f makefile.v3.mak
# touch include/cache.h
# make -f makefile.v3.mak
# make OUT_O_DIR=debug -f makefile.v3.mak
# make -f makefile.v3.mak testrun
# make -f makefile.v3.mak testrun -j4
# make OUT_O_DIR=debug -f makefile.v3.mak testrun -j4
# make OUT_O_DIR=debug -f makefile.v3.mak clean
# make -f makefile.v3.mak clean

ifeq ($(origin CC),default)
  CC = gcc
endif

CFLAGS ?= -O2
OUT_O_DIR ?= build
COMMONINC = -I./include
TESTS = ./Tests
SRC = ./source
ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

override CFLAGS += $(COMMONINC)

CSRC = LCmain.c source/cache.c source/hash.c source/list.c

# reproducing source tree in object tree
COBJ := $(addprefix $(OUT_O_DIR)/,$(CSRC:.c=.o))
DEPS = $(COBJ:.o=.d)

.PHONY: all
all: $(OUT_O_DIR)/LC.x

$(OUT_O_DIR)/LC.x: $(COBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

# static pattern rule to not redefine generic one
$(COBJ) : $(OUT_O_DIR)/%.o : %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(DEPS) : $(OUT_O_DIR)/%.d : %.c
	@mkdir -p $(@D)
	$(CC) -E $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@

TESTFILES=$(wildcard $(TESTS)/*.dat)

.PHONY: testrun
testrun: $(TESTFILES)

.PHONY: $(TESTFILES)
$(TESTFILES): $(OUT_O_DIR)/LC.x
	@$(ROOT_DIR)/runtest.sh $@ $(OUT_O_DIR)/LC.x

.PHONY: clean
clean:
	rm -rf $(COBJ) $(DEPS) $(OUT_O_DIR)/*.x $(OUT_O_DIR)/*.log

# targets which we have no need to recollect deps
NODEPS = clean

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
include $(DEPS)
endif