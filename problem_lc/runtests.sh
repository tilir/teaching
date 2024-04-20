#!/bin/bash

TESTS=$1

(
for i in ${TESTS}/*.dat
do
  echo "$(basename ${i})"
  ./LC.x < ${i};
  echo ""
done
) > all.log

diff -w all.log ${TESTS}/corr.log
