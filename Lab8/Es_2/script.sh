#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Usage: $0 <file>"
	exit 1
fi

maxlen=0
lineno=0
for line in `cat $1`; do
	let "lineno=lineno+1"
	len=`echo -n $line | wc -m | cut -d ' ' -f 1`
	if [ $len -gt $maxlen ]; then
		maxlen=$len
	fi
done

echo "Longest line is $maxlen chars long"
echo "The file has $lineno lines"

