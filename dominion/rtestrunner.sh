#!/bin/bash
seed=42

for i in card1 card2 adventurer; do
    rm -f cards.gcda dominion.gcda randomtest${i}.out
    echo "---- randomtest${i} ----" > ../randomtest${i}.out
    ./randomtest${i} $seed >> ../randomtest${i}.out 2>&1
    echo >> ../randomtest${i}.out
    gcov -o . ../dominion.c ../cards.c >> ../randomtest${i}.out
    cat cards.c.gcov >> ../randomtest${i}.out
    echo >> ../randomtest${i}.out
done