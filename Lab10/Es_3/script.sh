#!/bin/bash

if [ $# -ne 3 ]; then
  echo "Usage: $0 <d> <m> <y>"
  exit 1
fi

cal $1 $2 $3 | awk -f script.awk -v DAY=$1
