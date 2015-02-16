
#!/bin/bash

if [ $# -ne 1 ]
then
  echo "Usage: $0 <pid>"
  exit 1
fi

./script.awk -v pid=$1 input.txt
