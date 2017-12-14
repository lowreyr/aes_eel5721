#!/bin/bash

git pull
#module load aocl/17.0
#aoc -v -march=emulator device/$1.cl -o bin/$1.aocx
# if [ $? -ne 0 ]
# then
#   exit $?
# fi
make
if [ $? -ne 0 ]
then
  exit $?
fi
env CL_CONTEXT_EMULATOR_DEVICE_ALTERA=1 ./bin/host
