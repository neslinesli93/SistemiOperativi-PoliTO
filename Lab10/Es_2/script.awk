#!/bin/awk -f

{
  if ($2 == pid) {
    printf "Process %s, State %s", $8, $11
  }
}
