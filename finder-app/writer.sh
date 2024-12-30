#!/bin/bash
if [ $# -ne 2 ];
then
  echo "Wrong number of arguments"
  exit 1
fi
writefile=$1
writestr=$2

#the commands below might not be needed since $# == 2
if [ -z "$writefile" ]; then
  echo "Empty file name"
  exit 1
fi
if [ -d "$writefile" ];
then
  echo "This is a folder path"
  exit 1
fi
directory=$(dirname $writefile)
if [ ! -d "$directory" ];
then
  mkdir -p "$directory"
fi
touch "$writefile"
echo "${writestr}" > "$writefile"
