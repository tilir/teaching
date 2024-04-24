#------------------------------------------------------------------------------
#
# Makefile, approach 0
# * gcc is nailed down
# * includes repeated again and again
# * targets all, clean and testrun are weird
# * FLAGS is non-standard variable
#
#------------------------------------------------------------------------------
#
# This file is licensed after LGPL v3
# Look at: https://www.gnu.org/licenses/lgpl-3.0.en.html for details
#
#------------------------------------------------------------------------------

# make FLAGS="-g -O0"
# make clean
# make
# make testrun
# make clean

FLAGS ?= -O2
TESTS = ./Tests

all: LCmain.o cache.o hash.o list.o
	gcc $(FLAGS) -I./include LCmain.o cache.o hash.o list.o -o LC.x

LCmain.o: LCmain.c
	gcc $(FLAGS) -I./include -c LCmain.c

cache.o: ./source/cache.c
	gcc $(FLAGS) -I./include -c ./source/cache.c -o cache.o 

hash.o: ./source/hash.c
	gcc $(FLAGS) -I./include -c ./source/hash.c -o hash.o 

list.o: ./source/list.c
	gcc $(FLAGS) -I./include -c ./source/list.c -o list.o 

testrun: LC.x
	@for i in $(TESTS)/*.dat; do echo "$$(basename $${i})"; ./LC.x < $${i}; echo ""; done > all.log
	@if diff -w all.log $(TESTS)/corr.log; then echo "Tests pass"; else echo "Tests failed"; fi

clean:
	rm *.x
	rm -rf *.o
	rm -rf *.log

