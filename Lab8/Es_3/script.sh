#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Usage: $0 <dir>"
	exit 1
fi

for match in `ls $1`; do
	newname=`echo $match | tr "A-Z" "a-z"`
	if [ $newname != $match ]; then
		mv $1$match $1$newname
	fi
done
