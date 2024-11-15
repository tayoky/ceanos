#!/bin/bash

echo

echo "Recent Commits:"
git log --oneline -n 5

echo

echo "src folder disk usage:"
du -sh ../src

echo

echo "Number of files in src/:"
find ../src/ -type f | wc -l

echo

echo "Untracked Files:"
git ls-files --others --exclude-standard

echo