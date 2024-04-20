# try: make FLAGS="-g -O0"
FLAGS ?= -O2
TESTS = ./Tests

all: LCmain.o cache.o hash.o list.o
	gcc $(FLAGS) -I./include LCmain.o cache.o hash.o list.o -o LCsep.x

LCmain.o: LCmain.c
	gcc $(FLAGS) -I./include -c LCmain.c

cache.o: ./source/cache.c
	gcc $(FLAGS) -I./include -c ./source/cache.c -o cache.o 

hash.o: ./source/hash.c
	gcc $(FLAGS) -I./include -c ./source/hash.c -o hash.o 

list.o: ./source/list.c
	gcc $(FLAGS) -I./include -c ./source/list.c -o list.o 

testsep: LCsep.x
	for i in $(TESTS)/*.dat; do echo "$$(basename $${i})"; ./LCsep.x < $${i}; echo ""; done > all.log
	diff -q -w all.log $(TESTS)/corr.log || echo "Tests failed"

clean:
	rm *.x
	rm -rf *.o
