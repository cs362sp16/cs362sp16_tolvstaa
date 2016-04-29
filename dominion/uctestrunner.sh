#!/bin/bash

utest="unittest1 unittest2 unittest3 unittest4"
ctest="cardtest1 cardtest2 cardtest3 cardtest4"

for test in $utest $ctest; do
    rm -f dominion.gcda
    echo "---- $test ----"
    ./$test 2>&1
    echo
    gcov -o . -n ../dominion.c
    echo
done