#!/bin/bash

tr -d '\r' < autograde_tests/550000in1-1000000skew.txt > autograde_tests/550000in1-1000000skew.fixed.txt 
tr -d '\r' < autograde_tests/550000in1-1000000skew.ans > autograde_tests/550000in1-1000000skew.fixed.ans
rm autograde_tests/550000in1-1000000skew.txt autograde_tests/550000in1-1000000skew.ans
mv autograde_tests/550000in1-1000000skew.fixed.txt autograde_tests/550000in1-1000000skew.txt
mv autograde_tests/550000in1-1000000skew.fixed.ans autograde_tests/550000in1-1000000skew.ans

echo "This script edited: "
echo "  autograde_tests/550000in1-1000000skew.txt"
echo "  autograde_tests/550000in1-1000000skew.ans"
echo ""

echo "If ^M DOES NOT appear in the below output, the fix was successful."
echo "If you see a ^M character, contact course staff."
head -n 2 autograde_tests/550000in1-1000000skew.txt | cat -t
head -n 2 autograde_tests/550000in1-1000000skew.ans | cat -t

echo ""
echo "For these edits to be reflected in github, you will need to git add, git commit, and git push the changes"

