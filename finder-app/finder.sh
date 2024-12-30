#!/bin/bash
if [ $# -ne 2 ]; then 
  echo "Wrong number of arguments!"
  exit 1
fi
filesdir=$1
searchstr=$2

if [ ! -d $filesdir ]; then
  echo "${filesdir} is not a directory!"
  exit 1
fi

filecount=$(find "$filesdir" -type f | wc -l)
linecount=$(find "$filesdir" -type f -print0 | xargs -0 grep -h "$searchstr" | wc -l)
echo "The number of files are ${filecount} and the number of matching lines are ${linecount}"
