#!/bin/bash
sort lab06e04in.txt -k 1 -n -r
sort -k 2 lab06e04in.txt 
cat lab06e04in.txt | grep "2011/2012"
cat lab06e04in.txt | grep INF1T3 | sort -k 1 -n
cat lab06e04in.txt | grep -E "[0-9]+[24]\s" | sort -k 2
cat lab06e04in.txt | grep TLC1T3 | grep -i "da frequentare" | sort -n -k 1 -r
cat lab06e04in.txt | grep "[B-Z]*A[B-Z][B-Z]A[B-Z]*"
cat lab06e04in.txt | grep "[B-Z]*A[B-Z][B-Z]A[B-Z]*" | grep -v "\<A" | grep -v "\A>"
cat lab06e04in.txt | cut -c 1-6 | tr -d 0 | tr 8 1 | sort -n | uniq
cat lab06e04in.txt | cut -c 1-6 | tr -d 0 | tr 8 1 | sort -n | uniq -d | wc -l
