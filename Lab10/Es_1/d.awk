#!/bin/awk -f

END {
  system("sort -k 2 " ARGV[1])
}
