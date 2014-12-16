#!/bin/bash

# Exit if argc < 4
if [ $# -ne 3 ]; then
	echo "Usage: $0 <directory> <function> <output>"
	exit 1
fi

matches=`grep -H -n $2 $1*`
echo $matches > $3
cat $3 | sort -k 1 -o $3
