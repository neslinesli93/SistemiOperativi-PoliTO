#!/bin/bash

# Exit if argc < 4
if [ $# -ne 3 ]; then
	echo "Usage: $0 <directory> <function> <output>"
	exit 1
fi

# I am assuming that <directory> ends with a /
for file in `ls $1`; do
	for line in `grep -H -n $2 $1$file`; do
		echo $line
	done
done >> $3

sort $3 -k 1 -o $3
