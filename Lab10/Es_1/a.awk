#!/bin/awk -f

{
  if ($2 == 1) print $NF
}
