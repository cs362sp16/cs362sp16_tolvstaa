#!/bin/bash

utest="unittest1 unittest2 unittest3 unittest4"
ctest="cardtest1 cardtest2 cardtest3 cardtest4"

rm -f ../unittestresults.out

for test in $utest $ctest; do
    rm -f dominion.gcda
    echo "---- $test ----" >> ../unittestresults.out
    ./$test >> ../unittestresults.out 2>&1
    echo >> ../unittestresults.out
    gcov -o . -n ../dominion.c >> ../unittestresults.out
    echo >> ../unittestresults.out
done