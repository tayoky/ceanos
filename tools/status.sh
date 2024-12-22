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
untracked_files=$(git ls-files --others --exclude-standard)

if [ -z "$untracked_files" ]; then
    echo "none"
else
    echo "$untracked_files"
fi


echo
