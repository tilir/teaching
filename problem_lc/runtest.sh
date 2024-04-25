#!/bin/bash
#------------------------------------------------------------------------------
#
# Single test runner
#
#------------------------------------------------------------------------------

TEST=$1
TESTER=$2

ANS=${TEST%.*}.ans
NAME=$(basename $TEST)

eval ${TESTER} < $TEST > $NAME.log

DIFF=$(diff -w $NAME.log ${ANS})

if [ $? -ne 0 ]; then
  echo "" >> $NAME.log
  echo "Diff is:" >> $NAME.log
  echo "$DIFF" >> $NAME.log
  echo "Test ${NAME} failed, see ${NAME}.log"
  exit 1
else
  rm $NAME.log
  echo "Test ${NAME} passed"
fi
