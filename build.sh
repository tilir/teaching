#!/bin/bash

if [ -z ${FLAGS+x} ]
then
  echo "FLAGS unset, default to -O2"
  FLAGS="-O2"
fi

gcc $FLAGS -c -I./include LCmain.c -o LCmain.o
gcc $FLAGS -c -I./include ./source/cache.c -o cache.o
gcc $FLAGS -c -I./include ./source/hash.c -o hash.o
gcc $FLAGS -c -I./include ./source/list.c -o list.o
gcc $FLAGS LCmain.o cache.o hash.o list.o -o LCsep.x

