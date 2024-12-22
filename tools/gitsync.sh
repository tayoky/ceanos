#!/bin/bash

if git diff-index --quiet HEAD --; then
    echo "No changes to commit."
    exit 0
fi

git pull

git add .

echo "Enter commit message:"
read commit_message

while [ -z "$commit_message" ]; do
  echo "Commit message cannot be empty. Enter a message:"
  read commit_message
done

git commit -m "$commit_message"

git push

echo "Changes synced with remote repository!"
