#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Enter the filename: "
	read file
else
	file=$1
fi

totalwords=0
for line in `cat $file`; do
	for string in $line; do
		i=0
		found=0
		for word in ${words[*]}; do
			if [ $word = $string ]; then
				let "freq[i]=freq[i]+1"
				found=1
				break
			fi
			
			let "i=i+1"
		done

		if [ $found -eq 0 ]; then
			words[totalwords]=$string
			freq[totalwords]=1
			let "totalwords=totalwords+1"
		fi
	done
done

i=0
while [ $i -lt $totalwords ]; do
	printf "%s: %d\n" ${words[i]} ${freq[i]}
	let "i=i+1"
done
